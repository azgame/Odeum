﻿#include "Renderer.h"
#include "../Utilities/DXRHelper.h"
#include "../Utilities/DXRaytracingHelper.h"
#include "../dxc/dxcapi.h"
#include "ShaderHandler.h"
#include "SceneGraph.h"

#pragma region Renderer

Renderer::Renderer()
{
	m_deviceResources = nullptr;
}

Renderer::Renderer(const Renderer& other)
{
}

Renderer::~Renderer()
{
}

#pragma endregion

#pragma region Raster

bool Renderer::Initialize(int screenHeight, int screenWidth, HWND hwnd, std::vector<GameObject*> renderObjects)
{
	m_renderObjects = new std::vector<GameObject*>();

	for (auto o : renderObjects)
		m_renderObjects->push_back(o);

	if (dxrEnabled) {
		if (!InitializeRaytrace(screenHeight, screenWidth, hwnd)) return false;
	}
	else {
		if (!InitializeRaster(screenHeight, screenWidth, hwnd)) return false;
	}

	return true;
}

void Renderer::CreateWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera)
{
	if (dxrEnabled)
		CreateRaytracingWindowSizeDependentResources(screenHeight, screenWidth, camera);
	else
		CreateRasterWindowSizeDependentResources(screenHeight, screenWidth, camera);
}

bool Renderer::InitializeRaster(int screenHeight, int screenWidth, HWND hwnd)
{
	HRESULT result;

	if (!InitializeDeviceResources(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN, dxrEnabled)) return false;

	m_device = m_deviceResources->GetD3Device();
	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	DxShaderInfo vertexShader(L"Engine/Shaders/VertexShader.hlsl", "main", "vs_5_0");
	DxShaderInfo pixelShader(L"Engine/Shaders/PixelShader.hlsl", "main", "ps_5_0");

	ShaderHandler::GetInstance()->CreateShaderProgram(m_deviceResources, "BasicShader", vertexShader, pixelShader);

	m_shaderProgram = ShaderHandler::GetInstance()->GetShader("BasicShader");

	if (FAILED(m_shaderProgram->commandList->Reset(m_deviceResources->GetCommandAllocator(), m_shaderProgram->pipelineState)))
		Debug::Warning("Failed to reset command list", __FILENAME__, __LINE__);

	for (auto object : *m_renderObjects) {
		object->Initialize(m_device, m_shaderProgram->commandList);
	}

	// Close command list after render objects are recorded
	result = m_shaderProgram->commandList->Close();
	if (FAILED(result)) return false;

	// Load the command list array (only one command list for now)
	ID3D12CommandList* ppCommandLists[] = { m_shaderProgram->commandList };

	// Execute the list of commands
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	m_deviceResources->WaitForPrevFrame();

	// Heap descriptors for constant buffer data
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 3;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descHeap));
	if (FAILED(result)) return false;

	CreateCBResources();

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_descHeap->GetCPUDescriptorHandleForHeapStart();
	m_descHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Create the constant buffer view description
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.SizeInBytes = c_alignedConstantBufferSize * m_renderObjects->size();
	cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();

	m_device->CreateConstantBufferView(&cbvDesc, handle);

	handle.ptr += m_descHeapSize;
	
	m_device->CreateShaderResourceView(m_renderObjects->at(0)->GetModel()->GetMesh()->GetTextureBuffer(), &m_renderObjects->at(0)->GetModel()->GetMesh()->GetTextureBV(), handle);
	
	handle.ptr += m_descHeapSize;

	m_device->CreateShaderResourceView(m_renderObjects->at(0)->GetModel()->GetMesh(1)->GetTextureBuffer(), &m_renderObjects->at(0)->GetModel()->GetMesh(1)->GetTextureBV(), handle);

	return true;
}

void Renderer::CreateRasterWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera)
{
	m_camera = camera;

	float aspectRatio = (float)screenWidth / (float)screenHeight;
	float fovAngleY = 59.0f * DirectX::XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_deviceResources->GetViewPort();
	D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	camera->SetViewProperties(viewport, scissorRect);

	m_camera->SetProjMatrix(
		fovAngleY,
		aspectRatio,
		SCREEN_NEAR,
		SCREEN_DEPTH
	);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		DirectX::XMMatrixTranspose(m_camera->Projection())
	);

	static const DirectX::XMFLOAT3 eye = { 0.0f, 0.7f, 5.0f };
	static const DirectX::XMFLOAT3 at = { 0.0f, 0.0f, 0.0f };
	static const DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	
	m_camera->SetViewMatrix(eye, at, up);
	XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(m_camera->View()));
	XMStoreFloat4x4(&m_constantBufferData.model, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(0)));
}

bool Renderer::RenderRaster()
{
	HRESULT								result;
	D3D12_RESOURCE_BARRIER				barrier;
	unsigned int						renderTargetViewDescriptorSize;
	float								color[4];

	XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(m_camera->View()));

	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();
	
	// Reset the memory associated command allocator
	result = m_deviceResources->GetCommandAllocator()->Reset();
	if (FAILED(result)) return false;

	// Reset the command list
	result = m_shaderProgram->commandList->Reset(m_deviceResources->GetCommandAllocator(), m_shaderProgram->pipelineState);
	if (FAILED(result)) return false;

	m_shaderProgram->commandList->SetGraphicsRootSignature(m_shaderProgram->rootSignature);
	std::vector<ID3D12DescriptorHeap*> ppHeaps = { m_descHeap };
	m_shaderProgram->commandList->SetDescriptorHeaps(ppHeaps.size(), ppHeaps.data());

	m_shaderProgram->commandList->RSSetViewports(1, &m_camera->GetViewPort());
	m_shaderProgram->commandList->RSSetScissorRects(1, &m_camera->GetScissorRect());
	
	// Record commands in the command list now
	// Start by setting the resource barrier
	m_shaderProgram->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Get the render target view handle for the current back buffer
	renderTargetViewDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_deviceResources->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(), m_bufferIndex, renderTargetViewDescriptorSize);

	// Then set the color to clear the window to
	color[0] = 0.2;
	color[1] = 0.2;
	color[2] = 0.5;
	color[3] = 1.0;
	m_shaderProgram->commandList->ClearRenderTargetView(rtvHandle, color, 0, NULL);
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_deviceResources->GetDepthStencilView();
	m_shaderProgram->commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the back buffer as the render target
	m_shaderProgram->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &depthStencilView);

	CD3DX12_GPU_DESCRIPTOR_HANDLE handle(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0);

	std::vector<DirectX::XMFLOAT4> planes(6);
	m_camera->GetViewFrustum(planes);
	
	int outsideFrustum = 0;

	// Populate the command list - i.e. pass the command list to the objects in the scene (as given to the renderer)
	// and have the objects fill the command list with their resource data (buffer data)
	for (int i = 0; i < m_renderObjects->size(); i++)
	{
		if (!aabbOutsideFrustum(m_renderObjects->at(i)->position, m_renderObjects->at(i)->GetBoundingBox().maxVert, planes))
		{
			XMStoreFloat4x4(&m_constantBufferData.model, DirectX::XMMatrixTranspose(m_renderObjects->at(i)->GetModel()->GetTransform(0)));
			UINT8* destination = m_mappedConstantBuffer + (i * c_alignedConstantBufferSize);
			memcpy(destination, &m_constantBufferData, sizeof(m_constantBufferData));
			m_shaderProgram->commandList->SetGraphicsRootConstantBufferView(0, m_constantBuffer->GetGPUVirtualAddress() + (i * c_alignedConstantBufferSize));
			m_shaderProgram->commandList->SetGraphicsRootDescriptorTable(1, handle);
			m_renderObjects->at(i)->Render(m_shaderProgram->commandList);
			handle.Offset(m_descHeapSize); // if one object is frustum culled then the apple at the center will become red
		}	
		else
			outsideFrustum++;

		// handle.Offset(m_descHeapSize); // this is what we'd normally do
	}

	if (outsideFrustum == 1)
		Debug::Info("One object outside frustum", __FILENAME__, __LINE__); // break here to see moment of cull
	else if (outsideFrustum == 2)
		Debug::Info("Two objects outside frustum", __FILENAME__, __LINE__); // break here to see moment of cull

	// Indicate that the back buffer will now be used to present
	m_shaderProgram->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Close the list of commands
	result = m_shaderProgram->commandList->Close();
	if (FAILED(result)) return false;

	// Load the command list array (only one command list for now)
	std::vector<ID3D12CommandList*> ppCommandLists = { m_shaderProgram->commandList };

	// Execute the list of commands
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(ppCommandLists.size(), ppCommandLists.data());

	if (!m_deviceResources->Render()) return false;

	ppCommandLists.clear();
	ppHeaps.clear();

	return true;
}

// TODO - Aidan: Need to find a better home for this
bool Renderer::aabbOutsideFrustum(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 maxVert, std::vector<DirectX::XMFLOAT4> planes)
{
	for (int i = 0; i < 6; ++i)
		// if the box is completely behind any of the frustum planes, then it is outside the frustum
		if (aabbBehindPlane(position, maxVert, planes[i]))
			return true;

	return false;
}

bool Renderer::aabbBehindPlane(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 maxVert, DirectX::XMFLOAT4 plane)
{
	DirectX::SimpleMath::Vector3 n(abs(plane.x), abs(plane.y), abs(plane.z));

	float e = n.Dot(maxVert);

	DirectX::SimpleMath::Vector4 pos4(position.x, position.y, position.z, 1.0f);

	float s = pos4.Dot(plane);

	return (s + e) < 0.0f;
}

#pragma endregion

#pragma region Common

bool Renderer::InitializeDeviceResources(int screenHeight, int screenWidth, HWND hwnd, bool VSYNC_ENABLED, bool FULL_SCREEN, bool DXR_ENABLED)
{
	HRESULT result;

	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	m_deviceResources = new DeviceResources(); // need to get device and commandlist from device resources
	if (!m_deviceResources) return false;
	bool dxsuccess = m_deviceResources->Initialize(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN, DXR_ENABLED);
	if (!dxsuccess)
	{
		MessageBox(hwnd, L"Could not initialize DirectX", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Renderer::CreateCBResources()
{
	HRESULT result;

	// Init raster constant buffer
	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	size_t cbSize = m_renderObjects->size() * c_alignedConstantBufferSize;
	CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(cbSize);
	result = m_device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constantBuffer));
	if (FAILED(result)) return false;

	cbSize = c_frameCount * sizeof(AlignedSceneConstantBuffer);
	constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(cbSize);

	// Init ray scene constant buffer
	result = m_device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_dxrconstantBuffer));
	if (FAILED(result)) return false;

	cbSize = c_frameCount * sizeof(AlignedCubeConstantBuffer);
	constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(cbSize);

	// Init ray scene constant buffer
	result = m_device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_cubeConstantBuffer));
	if (FAILED(result)) return false;

	// Map the raster constant buffers.
	CD3DX12_RANGE readRange(0, 0);
	result = m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer));
	if (FAILED(result)) return false;
	

	// Map the ray constant buffers.
	result = m_dxrconstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_dxrmappedConstantBuffer));
	if (FAILED(result)) return false;

	// Map the cube constant buffers
	result = m_cubeConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_cubemappedConstantBuffer));

	return true;
}

void Renderer::Uninitialize()
{
	m_renderObjects->clear();
	if (m_renderObjects) delete m_renderObjects;
	SAFE_DELETE(m_camera);
	SAFE_RELEASE(m_cbvHeap);
	SAFE_RELEASE(m_constantBuffer);
	SAFE_DELETE(m_mappedConstantBuffer);
	SAFE_DELETE(m_dxrmappedConstantBuffer);
	SAFE_DELETE(m_cubemappedConstantBuffer);
	SAFE_RELEASE(m_dxrconstantBuffer);
	SAFE_RELEASE(m_cubeConstantBuffer);
	for (auto o : m_bottomLevelAccelerationStructure) SAFE_RELEASE(o);
	SAFE_RELEASE(instanceDescs);
	SAFE_RELEASE(m_topLevelAccelerationStructure);
	SAFE_RELEASE(tscratchResource);
	SAFE_RELEASE(m_rtStateObject);
	SAFE_RELEASE(m_rtStateObjectProps);
	SAFE_RELEASE(m_raytracingOutput);
	SAFE_RELEASE(m_shaderTable);
	SAFE_RELEASE(m_commandList);
	m_shaderProgram = nullptr;

	if (m_deviceResources != nullptr)
	{
		m_deviceResources->Uninitialize();
		delete m_deviceResources;
		m_deviceResources = nullptr;
	}

	SAFE_RELEASE(m_device);
}

bool Renderer::UpdateConstantResources()
{
	return true;
}

bool Renderer::Render()
{
	if (dxrEnabled) {
		if (!RenderRaytrace()) return false;
	}
	else
	{
		if (!RenderRaster()) return false;
	}

	return true;
}

#pragma endregion

#pragma region DXR

bool Renderer::InitializeRaytrace(int screenHeight, int screenWidth, HWND hwnd)
{
	HRESULT result;

	// Create raytracing interfaces: raytracing device and commandlist.
	if (!CreateRaytracingInterfaces(screenHeight, screenWidth, hwnd)) return false;

	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&m_commandList));
	if (FAILED(result)) return false;

	for (auto object : *m_renderObjects) {
		object->Initialize(m_device, m_commandList);
	}

	// Build raytracing bottom level acceleration structures from the generated geometry.
	if (!BuildBottomLevelAccelerationStructures()) return false;

	// Build raytracing top level acceleration structure for first frame
	if (!BuildTopLevelAccelerationStructures(false)) return false;

	// Create a heap for descriptors.
	if (!CreateDescriptorHeap()) return false;

	// Create a raytracing pipeline state object which defines the binding of shaders, state and resources to be used during raytracing.
	if (!CreateRaytracingPipelineStateObject()) return false;

	// Build shader tables, which define shaders and their local root arguments.
	if (!BuildShaderTables()) return false;

	m_commandList->Close();
	ID3D12CommandList* pGraphicsList[] = { m_commandList };
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(pGraphicsList), pGraphicsList);

	if (!m_deviceResources->Render()) return false;

	return true;
}

bool Renderer::CreateRaytracingInterfaces(int screenHeight, int screenWidth, HWND hwnd)
{
	if (!InitializeDeviceResources(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN, dxrEnabled)) return false;

	m_device = m_deviceResources->GetD3Device();

	return true;
}

void Renderer::CreateRaytracingWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera)
{
	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	m_cubeCB.albedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_camera = camera;

	float aspectRatio = screenWidth / screenHeight;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_deviceResources->GetViewPort();
	D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	m_camera->SetViewProperties(viewport, scissorRect);

	// Camera->SetProjMatrix
	m_camera->SetProjMatrix(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		DirectX::XMMatrixTranspose(m_camera->Projection())
	);

	// Camera->SetViewMatrix
	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis. ------ just starting location, can change
	static const DirectX::XMFLOAT4 eye = { 0.0f, 0.7f, 1.5f, 1.0f };
	static const DirectX::XMFLOAT3 at = { 0.0f, -0.1f, 0.0f };
	static const DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

	m_camera->SetViewMatrix(DirectX::XMFLOAT3(eye.x, eye.y, eye.z), at, up);
	DirectX::XMMATRIX viewProj = m_camera->View() * m_camera->Projection();
	XMStoreFloat4x4(&m_sceneCB[m_bufferIndex].projection, DirectX::XMMatrixTranspose(viewProj));
	m_sceneCB[m_bufferIndex].eye = eye;

	m_sceneCB[m_bufferIndex].lightPos = DirectX::XMFLOAT4(0.0f, 5.0f, 0.0f, 0.0f);
	m_sceneCB[m_bufferIndex].lightAmbient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_sceneCB[m_bufferIndex].lightDiffuse = DirectX::XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f);

	for (auto& sceneCB : m_sceneCB)
	{
		sceneCB = m_sceneCB[m_bufferIndex];
	}
}

bool Renderer::BuildBottomLevelAccelerationStructures()
{
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometryDesc(m_renderObjects->size());
	m_bottomLevelAccelerationStructure.resize(m_renderObjects->size());

	for (int i = 0; i < m_renderObjects->size(); i++) {
		geometryDesc[i].Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
		geometryDesc[i].Triangles.IndexBuffer = m_renderObjects->at(i)->GetModel()->GetMesh()->GetIndexBuffer()->GetGPUVirtualAddress();
		geometryDesc[i].Triangles.IndexCount = static_cast<UINT>(m_renderObjects->at(i)->GetModel()->GetMesh()->GetIndexCount());
		geometryDesc[i].Triangles.IndexFormat = m_renderObjects->at(i)->GetModel()->GetMesh()->GetIndexBV().Format;
		geometryDesc[i].Triangles.Transform3x4 = 0;
		geometryDesc[i].Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		geometryDesc[i].Triangles.VertexCount = static_cast<UINT>(m_renderObjects->at(i)->GetModel()->GetMesh()->GetVertexCount());
		geometryDesc[i].Triangles.VertexBuffer.StartAddress = m_renderObjects->at(i)->GetModel()->GetMesh()->GetVertexBuffer()->GetGPUVirtualAddress();
		geometryDesc[i].Triangles.VertexBuffer.StrideInBytes = sizeof(Vertex);
		geometryDesc[i].Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
		
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelPrebuildInfo = {};
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS bottomLevelInputs = {};
		bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
		bottomLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
		bottomLevelInputs.pGeometryDescs = &geometryDesc[i];
		bottomLevelInputs.NumDescs = 1;
		bottomLevelInputs.Flags = buildFlags;

		m_device->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &bottomLevelPrebuildInfo);

		bottomLevelPrebuildInfo.ScratchDataSizeInBytes = ALIGN(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT, bottomLevelPrebuildInfo.ScratchDataSizeInBytes);
		bottomLevelPrebuildInfo.ResultDataMaxSizeInBytes = ALIGN(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT, bottomLevelPrebuildInfo.ResultDataMaxSizeInBytes);

		ID3D12Resource* bscratchResource;
		AllocateUAVBuffer(m_device, bottomLevelPrebuildInfo.ScratchDataSizeInBytes, &bscratchResource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"Scratch Resource");
		bscratchResource->SetName(L"DXR BLAS Scratch");

		D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;

		AllocateUAVBuffer(m_device, bottomLevelPrebuildInfo.ResultDataMaxSizeInBytes, &m_bottomLevelAccelerationStructure[i], initialResourceState, L"BottomLevelAccelerationStructure");
		m_bottomLevelAccelerationStructure[i]->SetName(L"DXR BLAS");

		// Bottom Level Acceleration Structure Desc
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC bottomLevelBuildDesc = {};
		bottomLevelBuildDesc.Inputs = bottomLevelInputs;
		bottomLevelBuildDesc.ScratchAccelerationStructureData = bscratchResource->GetGPUVirtualAddress();
		bottomLevelBuildDesc.DestAccelerationStructureData = m_bottomLevelAccelerationStructure[i]->GetGPUVirtualAddress();

		m_commandList->BuildRaytracingAccelerationStructure(&bottomLevelBuildDesc, 0, nullptr);

		D3D12_RESOURCE_BARRIER uavBarrier;
		uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		uavBarrier.UAV.pResource = m_bottomLevelAccelerationStructure[i];
		uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		m_commandList->ResourceBarrier(1, &uavBarrier);
	}

	return true;
}

bool Renderer::BuildTopLevelAccelerationStructures(bool update)
{
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;

	UINT64 bufferSize = static_cast<UINT64>(m_renderObjects->size() * sizeof(D3D12_RAYTRACING_INSTANCE_DESC));

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS topLevelInputs = {};
	topLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	topLevelInputs.Flags = buildFlags;
	topLevelInputs.NumDescs = m_renderObjects->size();	
	topLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO topLevelPrebuildInfo = {};
	m_device->GetRaytracingAccelerationStructurePrebuildInfo(&topLevelInputs, &topLevelPrebuildInfo);

	topLevelPrebuildInfo.ScratchDataSizeInBytes = ALIGN(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT, topLevelPrebuildInfo.ScratchDataSizeInBytes);
	topLevelPrebuildInfo.ResultDataMaxSizeInBytes = ALIGN(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT, topLevelPrebuildInfo.ResultDataMaxSizeInBytes);

	if (update)
	{
		D3D12_RESOURCE_BARRIER uavBarrier = {};
		uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		uavBarrier.UAV.pResource = m_topLevelAccelerationStructure;
		m_commandList->ResourceBarrier(1, &uavBarrier);
	}
	else
	{
		AllocateUAVBuffer(m_device, topLevelPrebuildInfo.ScratchDataSizeInBytes, &tscratchResource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"Scratch Resource");
		tscratchResource->SetName(L"DXR TLAS Scratch");

		D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
		AllocateUAVBuffer(m_device, topLevelPrebuildInfo.ResultDataMaxSizeInBytes, &m_topLevelAccelerationStructure, initialResourceState, L"TopLevelAccelerationStructure");
		m_topLevelAccelerationStructure->SetName(L"DXR TLAS");

		m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&instanceDescs));

		instanceDescs->SetName(L"DXR TLAS Instance Descriptors");
	}

	D3D12_RAYTRACING_INSTANCE_DESC* instanceDesc;
	instanceDescs->Map(0, nullptr, (void**)&instanceDesc);
	ZeroMemory(instanceDesc, bufferSize);

	for (int i = 0; i < m_renderObjects->size(); i++) {
		// Create an instance desc for the bottom-level acceleration structure
		DirectX::XMStoreFloat3x4(reinterpret_cast<DirectX::XMFLOAT3X4*>(instanceDesc[i].Transform), m_renderObjects->at(i)->GetModel()->GetTransform(0));
		instanceDesc[i].InstanceMask = 1;
		//instanceDesc.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE;
		instanceDesc[i].AccelerationStructure = m_bottomLevelAccelerationStructure[i]->GetGPUVirtualAddress();
	}

	instanceDescs->Unmap(0, nullptr);

	// Top Level Acceleration Structure Desc
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC topLevelBuildDesc = {};
	topLevelBuildDesc.Inputs = topLevelInputs;
	topLevelBuildDesc.Inputs.InstanceDescs = instanceDescs->GetGPUVirtualAddress();
	topLevelBuildDesc.ScratchAccelerationStructureData = tscratchResource->GetGPUVirtualAddress();
	topLevelBuildDesc.DestAccelerationStructureData = m_topLevelAccelerationStructure->GetGPUVirtualAddress();

	if (update)
	{
		topLevelBuildDesc.Inputs.Flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
		topLevelBuildDesc.SourceAccelerationStructureData = m_topLevelAccelerationStructure->GetGPUVirtualAddress();
	}
	
	m_commandList->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);

	D3D12_RESOURCE_BARRIER uavBarrier;
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = m_topLevelAccelerationStructure;
	uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_commandList->ResourceBarrier(1, &uavBarrier);

	return true;
}

bool Renderer::SerializeAndCreateRaytracingRootSignature(D3D12_ROOT_SIGNATURE_DESC& desc, ID3D12RootSignature** rootSig)
{
	HRESULT result;

	ID3DBlob* blob;
	ID3DBlob* error;

	result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);
	if (FAILED(result)) return false;

	result = m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(rootSig));
	if (FAILED(result)) return false;

	return true;
}

bool Renderer::CreateRaytracingPipelineStateObject()
{
	HRESULT result;

	D3D12ShaderCompilerInfo shaderCompiler;
	result = shaderCompiler.DxcDllHelper.Initialize();
	shaderCompiler.DxcDllHelper.CreateInstance(CLSID_DxcCompiler, &shaderCompiler.compiler);
	shaderCompiler.DxcDllHelper.CreateInstance(CLSID_DxcLibrary, &shaderCompiler.library);

	UINT32 code(0);
	IDxcBlobEncoding* pRaygen(nullptr);
	IDxcBlobEncoding* pHit(nullptr);
	IDxcBlobEncoding* pMiss(nullptr);

	dxr.rgs = RtProgram(D3D12ShaderInfo(L"Engine/Shaders/Raygen.hlsl", L"", L"lib_6_3"));
	dxr.miss = RtProgram(D3D12ShaderInfo(L"Engine/Shaders/Miss.hlsl", L"", L"lib_6_3"));
	dxr.hit = HitProgram(L"Hit");
	dxr.hit.chs = RtProgram(D3D12ShaderInfo(L"Engine/Shaders/Hit.hlsl", L"", L"lib_6_3"));

	shaderCompiler.library->CreateBlobFromFile(dxr.rgs.info.filename, &code, &pRaygen);
	IDxcIncludeHandler* dxcIncludeHandler;
	shaderCompiler.library->CreateIncludeHandler(&dxcIncludeHandler);

	IDxcOperationResult* pResult;

	result = shaderCompiler.compiler->Compile(
		pRaygen,
		dxr.rgs.info.filename,
		dxr.rgs.info.entryPoint,
		dxr.rgs.info.targetProfile,
		dxr.rgs.info.arguments,
		dxr.rgs.info.argCount,
		dxr.rgs.info.defines,
		dxr.rgs.info.defineCount,
		dxcIncludeHandler,
		&pResult
	);

	result = pResult->GetResult(&dxr.rgs.blob);

	if (FAILED(result)) {
		IDxcBlobEncoding* error;
		result = pResult->GetErrorBuffer(&error);

		// Convert error blob to a string
		std::vector<char> infoLog(error->GetBufferSize() + 1);
		memcpy(infoLog.data(), error->GetBufferPointer(), error->GetBufferSize());
		infoLog[error->GetBufferSize()] = 0;

		std::string errorMsg = "Shader Compiler Error:\n";
		errorMsg.append(infoLog.data());

		MessageBoxA(nullptr, errorMsg.c_str(), "Error!", MB_OK);
	}

	dxr.rgs.SetBytecode();

	// Describe the ray generation root signature
	D3D12_DESCRIPTOR_RANGE ranges[3];

	ranges[0].BaseShaderRegister = 0;
	ranges[0].NumDescriptors = 2;
	ranges[0].RegisterSpace = 0;
	ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	ranges[0].OffsetInDescriptorsFromTableStart = 0;

	ranges[1].BaseShaderRegister = 0;
	ranges[1].NumDescriptors = 1;
	ranges[1].RegisterSpace = 0;
	ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	ranges[1].OffsetInDescriptorsFromTableStart = 2;

	ranges[2].BaseShaderRegister = 0;
	ranges[2].NumDescriptors = 4;
	ranges[2].RegisterSpace = 0;
	ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	ranges[2].OffsetInDescriptorsFromTableStart = 3;

	D3D12_ROOT_PARAMETER param0 = {};
	param0.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param0.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	param0.DescriptorTable.NumDescriptorRanges = _countof(ranges);
	param0.DescriptorTable.pDescriptorRanges = ranges;

	D3D12_ROOT_PARAMETER rootParams[] = { param0 };

	D3D12_ROOT_SIGNATURE_DESC rootDesc = {};
	rootDesc.NumParameters = _countof(rootParams);
	rootDesc.pParameters = rootParams;
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

	// Create the root signature
	SerializeAndCreateRaytracingRootSignature(rootDesc, &dxr.rgs.pRootSignature);

	shaderCompiler.library->CreateBlobFromFile(dxr.miss.info.filename, &code, &pMiss);

	result = shaderCompiler.compiler->Compile(
		pMiss,
		dxr.miss.info.filename,
		dxr.miss.info.entryPoint,
		dxr.miss.info.targetProfile,
		dxr.miss.info.arguments,
		dxr.miss.info.argCount,
		dxr.miss.info.defines,
		dxr.miss.info.defineCount,
		dxcIncludeHandler,
		&pResult
	);

	result = pResult->GetResult(&dxr.miss.blob);

	if (FAILED(result)) {
		IDxcBlobEncoding* error;
		result = pResult->GetErrorBuffer(&error);

		// Convert error blob to a string
		std::vector<char> infoLog(error->GetBufferSize() + 1);
		memcpy(infoLog.data(), error->GetBufferPointer(), error->GetBufferSize());
		infoLog[error->GetBufferSize()] = 0;

		std::string errorMsg = "Shader Compiler Error:\n";
		errorMsg.append(infoLog.data());

		MessageBoxA(nullptr, errorMsg.c_str(), "Error!", MB_OK);
	}

	dxr.miss.SetBytecode();

	shaderCompiler.library->CreateBlobFromFile(dxr.hit.chs.info.filename, &code, &pHit);

	result = shaderCompiler.compiler->Compile(
		pHit,
		dxr.hit.chs.info.filename,
		dxr.hit.chs.info.entryPoint,
		dxr.hit.chs.info.targetProfile,
		dxr.hit.chs.info.arguments,
		dxr.hit.chs.info.argCount,
		dxr.hit.chs.info.defines,
		dxr.hit.chs.info.defineCount,
		dxcIncludeHandler,
		&pResult
	);

	result = pResult->GetResult(&dxr.hit.chs.blob);

	if (FAILED(result)) {
		IDxcBlobEncoding* error;
		result = pResult->GetErrorBuffer(&error);

		// Convert error blob to a string
		std::vector<char> infoLog(error->GetBufferSize() + 1);
		memcpy(infoLog.data(), error->GetBufferPointer(), error->GetBufferSize());
		infoLog[error->GetBufferSize()] = 0;

		std::string errorMsg = "Shader Compiler Error:\n";
		errorMsg.append(infoLog.data());

		MessageBoxA(nullptr, errorMsg.c_str(), "Error!", MB_OK);
	}

	dxr.hit.chs.SetBytecode();

	// Need 10 subobjects:
	// 1 for RGS program
	// 1 for Miss program
	// 1 for CHS program
	// 1 for Hit Group
	// 2 for RayGen Root Signature (root-signature and association)
	// 2 for Shader Config (config and association)
	// 1 for Global Root Signature
	// 1 for Pipeline Config
	UINT index = 0;
	std::vector<D3D12_STATE_SUBOBJECT> subobjects;
	subobjects.resize(10);

	// Add state subobject for the RGS
	D3D12_EXPORT_DESC rgsExportDesc = {};
	rgsExportDesc.Name = L"RayGen_12";
	rgsExportDesc.ExportToRename = L"MyRaygenShader";
	rgsExportDesc.Flags = D3D12_EXPORT_FLAG_NONE;

	D3D12_DXIL_LIBRARY_DESC	rgsLibDesc = {};
	rgsLibDesc.DXILLibrary.BytecodeLength = dxr.rgs.blob->GetBufferSize();
	rgsLibDesc.DXILLibrary.pShaderBytecode = dxr.rgs.blob->GetBufferPointer();
	rgsLibDesc.NumExports = 1;
	rgsLibDesc.pExports = &rgsExportDesc;

	D3D12_STATE_SUBOBJECT rgslib = {};
	rgslib.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	rgslib.pDesc = &rgsLibDesc;

	subobjects[index++] = rgslib;

	// Add state subobject for the Miss shader
	D3D12_EXPORT_DESC msExportDesc = {};
	msExportDesc.Name = L"Miss_5";
	msExportDesc.ExportToRename = L"MyMissShader";
	msExportDesc.Flags = D3D12_EXPORT_FLAG_NONE;

	D3D12_DXIL_LIBRARY_DESC	msLibDesc = {};
	msLibDesc.DXILLibrary.BytecodeLength = dxr.miss.blob->GetBufferSize();
	msLibDesc.DXILLibrary.pShaderBytecode = dxr.miss.blob->GetBufferPointer();
	msLibDesc.NumExports = 1;
	msLibDesc.pExports = &msExportDesc;

	D3D12_STATE_SUBOBJECT ms = {};
	ms.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	ms.pDesc = &msLibDesc;

	subobjects[index++] = ms;

	// Add state subobject for the Closest Hit shader
	D3D12_EXPORT_DESC chsExportDesc = {};
	chsExportDesc.Name = L"ClosestHit_76";
	chsExportDesc.ExportToRename = L"MyClosestHitShader";
	chsExportDesc.Flags = D3D12_EXPORT_FLAG_NONE;

	D3D12_DXIL_LIBRARY_DESC	chsLibDesc = {};
	chsLibDesc.DXILLibrary.BytecodeLength = dxr.hit.chs.blob->GetBufferSize();
	chsLibDesc.DXILLibrary.pShaderBytecode = dxr.hit.chs.blob->GetBufferPointer();
	chsLibDesc.NumExports = 1;
	chsLibDesc.pExports = &chsExportDesc;

	D3D12_STATE_SUBOBJECT chs = {};
	chs.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	chs.pDesc = &chsLibDesc;

	subobjects[index++] = chs;

	// Add a state subobject for the hit group
	D3D12_HIT_GROUP_DESC hitGroupDesc = {};
	hitGroupDesc.ClosestHitShaderImport = L"ClosestHit_76";
	hitGroupDesc.HitGroupExport = L"HitGroup";

	D3D12_STATE_SUBOBJECT hitGroup = {};
	hitGroup.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
	hitGroup.pDesc = &hitGroupDesc;

	subobjects[index++] = hitGroup;

	// Add a state subobject for the shader payload configuration
	D3D12_RAYTRACING_SHADER_CONFIG shaderDesc = {};
	shaderDesc.MaxPayloadSizeInBytes = sizeof(DirectX::XMFLOAT4);	// RGB and HitT
	shaderDesc.MaxAttributeSizeInBytes = sizeof(DirectX::XMFLOAT2);

	D3D12_STATE_SUBOBJECT shaderConfigObject = {};
	shaderConfigObject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
	shaderConfigObject.pDesc = &shaderDesc;

	subobjects[index++] = shaderConfigObject;

	// Create a list of the shader export names that use the payload
	const WCHAR* shaderExports[] = { L"RayGen_12", L"Miss_5", L"HitGroup" };

	// Add a state subobject for the association between shaders and the payload
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION shaderPayloadAssociation = {};
	shaderPayloadAssociation.NumExports = _countof(shaderExports);
	shaderPayloadAssociation.pExports = shaderExports;
	shaderPayloadAssociation.pSubobjectToAssociate = &subobjects[(index - 1)];

	D3D12_STATE_SUBOBJECT shaderPayloadAssociationObject = {};
	shaderPayloadAssociationObject.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	shaderPayloadAssociationObject.pDesc = &shaderPayloadAssociation;

	subobjects[index++] = shaderPayloadAssociationObject;

	// Add a state subobject for the shared root signature
	D3D12_STATE_SUBOBJECT rayGenRootSigObject = {};
	rayGenRootSigObject.Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
	rayGenRootSigObject.pDesc = &dxr.rgs.pRootSignature;

	subobjects[index++] = rayGenRootSigObject;

	// Create a list of the shader export names that use the root signature
	const WCHAR* rootSigExports[] = { L"RayGen_12", L"HitGroup", L"Miss_5" };

	// Add a state subobject for the association between the RayGen shader and the local root signature
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION rayGenShaderRootSigAssociation = {};
	rayGenShaderRootSigAssociation.NumExports = _countof(rootSigExports);
	rayGenShaderRootSigAssociation.pExports = rootSigExports;
	rayGenShaderRootSigAssociation.pSubobjectToAssociate = &subobjects[(index - 1)];

	D3D12_STATE_SUBOBJECT rayGenShaderRootSigAssociationObject = {};
	rayGenShaderRootSigAssociationObject.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	rayGenShaderRootSigAssociationObject.pDesc = &rayGenShaderRootSigAssociation;

	subobjects[index++] = rayGenShaderRootSigAssociationObject;

	D3D12_STATE_SUBOBJECT globalRootSig;
	globalRootSig.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
	globalRootSig.pDesc = &dxr.miss.pRootSignature;

	subobjects[index++] = globalRootSig;

	// Add a state subobject for the ray tracing pipeline config
	D3D12_RAYTRACING_PIPELINE_CONFIG pipelineConfig = {};
	pipelineConfig.MaxTraceRecursionDepth = 1;

	D3D12_STATE_SUBOBJECT pipelineConfigObject = {};
	pipelineConfigObject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
	pipelineConfigObject.pDesc = &pipelineConfig;

	subobjects[index++] = pipelineConfigObject;

	// Describe the Ray Tracing Pipeline State Object
	D3D12_STATE_OBJECT_DESC pipelineDesc = {};
	pipelineDesc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
	pipelineDesc.NumSubobjects = static_cast<UINT>(subobjects.size());
	pipelineDesc.pSubobjects = subobjects.data();

	PrintStateObjectDesc(&pipelineDesc);

	// Create the RT Pipeline State Object (RTPSO)
	result = m_device->CreateStateObject(&pipelineDesc, IID_PPV_ARGS(&m_rtStateObject));
	if (FAILED(result)) return false;
	m_rtStateObject->SetName(L"DXR Pipeline State Object");

	ThrowIfFailed(m_rtStateObject->QueryInterface(&m_rtStateObjectProps));

	return true;
}

bool Renderer::CreateDescriptorHeap()
{
	HRESULT result;

	// Describe the CBV/SRV/UAV heap
	// Need 6 entries:
	// 1 CBV for the Scene
	// 1 CBV for the Cube
	// 1 UAV for the RT output
	// 1 SRV for the Scene BVH
	// 1 SRV for the index buffer
	// 1 SRV for the vertex buffer
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 4 + (3 * m_renderObjects->size());
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// Create descriptor heap
	m_device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_descHeap));

	CreateCBResources();

	CreateDescHeapViews();

	return true;
}

bool Renderer::CreateDescHeapViews()
{
	// Create the handle and get the heap size for increment
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_descHeap->GetCPUDescriptorHandleForHeapStart();
	m_descHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Create the constant buffer view description
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.SizeInBytes = ALIGN(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, sizeof(SceneConstantBuffer)) * c_frameCount;
	cbvDesc.BufferLocation = m_dxrconstantBuffer->GetGPUVirtualAddress();

	m_device->CreateConstantBufferView(&cbvDesc, handle);

	// Create the cube constant buffer view description
	D3D12_CONSTANT_BUFFER_VIEW_DESC cubeDesc = {};
	cubeDesc.SizeInBytes = ALIGN(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, sizeof(CubeConstantBuffer)) * c_frameCount;
	cubeDesc.BufferLocation = m_cubeConstantBuffer->GetGPUVirtualAddress();

	handle.ptr += m_descHeapSize;
	m_device->CreateConstantBufferView(&cubeDesc, handle);

	// Create the output resource. Dimensions and format should match swapChain
	CD3DX12_RESOURCE_DESC uavDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, m_screenWidth, m_screenHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	HRESULT result = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&uavDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&m_raytracingOutput)
	);

	if (FAILED(result)) return false;

	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

	handle.ptr += m_descHeapSize;
	m_device->CreateUnorderedAccessView(m_raytracingOutput, nullptr, &UAVDesc, handle);

	// Create the dxr tlas srv
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = m_topLevelAccelerationStructure->GetGPUVirtualAddress();

	handle.ptr += m_descHeapSize;
	m_device->CreateShaderResourceView(nullptr, &srvDesc, handle);

	for (auto object : *m_renderObjects) {
		// Create the index buffer srv
		D3D12_SHADER_RESOURCE_VIEW_DESC indexSRVDesc;
		indexSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		indexSRVDesc.Format = DXGI_FORMAT_R16_UINT;
		indexSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		indexSRVDesc.Buffer.StructureByteStride = 0;
		indexSRVDesc.Buffer.FirstElement = 0;
		indexSRVDesc.Buffer.NumElements = object->GetModel()->GetMesh()->GetIndexCount();
		indexSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		handle.ptr += m_descHeapSize;
		m_device->CreateShaderResourceView(object->GetModel()->GetMesh()->GetIndexBuffer(), &indexSRVDesc, handle);

		// Create the vertex buffer SRV
		D3D12_SHADER_RESOURCE_VIEW_DESC vertexSRVDesc;
		vertexSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		vertexSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		vertexSRVDesc.Buffer.StructureByteStride = sizeof(Vertex);
		vertexSRVDesc.Buffer.FirstElement = 0;
		vertexSRVDesc.Buffer.NumElements = object->GetModel()->GetMesh()->GetVertexCount();
		vertexSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		handle.ptr += m_descHeapSize;
		m_device->CreateShaderResourceView(object->GetModel()->GetMesh()->GetVertexBuffer(), &vertexSRVDesc, handle);

		handle.ptr += m_descHeapSize;
		m_device->CreateShaderResourceView(object->GetModel()->GetMesh()->GetTextureBuffer(), &object->GetModel()->GetMesh()->GetTextureBV(), handle);
	}
	
	return true;
}

bool Renderer::BuildShaderTables()
{
	/*
	The Shader Table layout is as follows:
		Entry 0 - Ray Generation shader
		Entry 1 - Miss shader
		Entry 2 - Closest Hit shader
	All shader records in the Shader Table must have the same size, so shader record size will be based on the largest required entry.
	The ray generation program requires the largest entry:
		32 bytes - D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES
	  +  8 bytes - a CBV/SRV/UAV descriptor table pointer (64-bits)
	  = 40 bytes ->> aligns to 64 bytes
	The entry size must be aligned up to D3D12_RAYTRACING_SHADER_BINDING_TABLE_RECORD_BYTE_ALIGNMENT
	*/

	HRESULT result;

	uint32_t shaderIdSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	uint32_t shaderTableSize = 0;

	m_shaderTableRecordSize = shaderIdSize;
	m_shaderTableRecordSize += 8;							// CBV/SRV/UAV descriptor table
	m_shaderTableRecordSize = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, m_shaderTableRecordSize);

	shaderTableSize = (m_shaderTableRecordSize * 3);		// 3 shader records in the table
	shaderTableSize = ALIGN(D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT, shaderTableSize);

	// Create the shader table buffer
	CD3DX12_RESOURCE_DESC m_shaderDesc = CD3DX12_RESOURCE_DESC::Buffer(shaderTableSize);

	result = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&m_shaderDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_shaderTable)
	);

	ThrowIfFailed(result, L"Error: Could not create shader table!");

	m_shaderTable->SetName(L"DXR Shader Table");

	// Map the buffer
	uint8_t* pData;
	CD3DX12_RANGE readRange(0, 0);
	result = m_shaderTable->Map(0, &readRange, (void**)&pData);
	if (FAILED(result)) return false;

	// Shader Record 0 - Ray Generation program and local root parameter data (descriptor table with constant buffer and IB/VB pointers)
	memcpy(pData, m_rtStateObjectProps->GetShaderIdentifier(L"RayGen_12"), shaderIdSize);

	// Set the root parameter data. Point to start of descriptor heap.
	*reinterpret_cast<D3D12_GPU_DESCRIPTOR_HANDLE*>(pData + shaderIdSize) = m_descHeap->GetGPUDescriptorHandleForHeapStart();

	// Shader Record 1 - Miss program (no local root arguments to set)
	pData += m_shaderTableRecordSize;
	memcpy(pData, m_rtStateObjectProps->GetShaderIdentifier(L"Miss_5"), shaderIdSize);

	// Shader Record 2 - Closest Hit program and local root parameter data (descriptor table with constant buffer and IB/VB pointers)
	pData += m_shaderTableRecordSize;
	memcpy(pData, m_rtStateObjectProps->GetShaderIdentifier(L"HitGroup"), shaderIdSize);

	// Set the root parameter data. Point to start of descriptor heap.
	*reinterpret_cast<D3D12_GPU_DESCRIPTOR_HANDLE*>(pData + shaderIdSize) = m_descHeap->GetGPUDescriptorHandleForHeapStart();

	// Unmap
	m_shaderTable->Unmap(0, nullptr);

	return true;
}

bool Renderer::RenderRaytrace()
{
	HRESULT result;

	// Reset command list and allocator.
	result = m_deviceResources->GetCommandAllocator()->Reset();
	if (FAILED(result)) return false;
	ThrowIfFailed(m_commandList->Reset(m_deviceResources->GetCommandAllocator(), nullptr));

	BuildTopLevelAccelerationStructures(true);

	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	m_sceneCB[m_bufferIndex].eye = DirectX::XMFLOAT4(m_camera->Eye().x, m_camera->Eye().y, m_camera->Eye().z, 1.0f);
	XMStoreFloat4x4(&m_sceneCB[m_bufferIndex].projection, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_camera->View() * m_camera->Projection())));

	memcpy(&m_dxrmappedConstantBuffer[m_bufferIndex].constants, &m_sceneCB[m_bufferIndex], sizeof(m_sceneCB[m_bufferIndex]));
	memcpy(&m_cubemappedConstantBuffer->constants, &m_cubeCB, sizeof(m_cubeCB));

	D3D12_RESOURCE_BARRIER preCopyBarriers[2];
	preCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	preCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_raytracingOutput, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	m_commandList->ResourceBarrier(_countof(preCopyBarriers), preCopyBarriers);

	DoRaytracing();
	
	D3D12_RESOURCE_BARRIER postCopyBarriers[2];

	postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_raytracingOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);

	m_commandList->ResourceBarrier(1, &postCopyBarriers[1]);

	m_commandList->CopyResource(m_deviceResources->GetBackBuffer(m_bufferIndex), m_raytracingOutput);

	postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &postCopyBarriers[0]);

	// Execute command lists
	m_commandList->Close();
	ID3D12CommandList* commandLists[] = { m_commandList };
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(commandLists), commandLists);

	if (!m_deviceResources->Render()) return false;

	return true;
}

bool Renderer::DoRaytracing()
{
	// Bind the heaps, acceleration structure and dispatch rays
	//CreateDescHeapViews();
	// TODO - Aidan: rewrite desc heaps for memory management
	ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	m_commandList->SetGraphicsRootSignature(dxr.rgs.pRootSignature);

	D3D12_DISPATCH_RAYS_DESC dispatchDesc = {};

	// Since each shader table has only one shader record, the stride is same as the size.
	dispatchDesc.RayGenerationShaderRecord.StartAddress = m_shaderTable->GetGPUVirtualAddress();
	dispatchDesc.RayGenerationShaderRecord.SizeInBytes = m_shaderTableRecordSize;

	dispatchDesc.MissShaderTable.StartAddress = m_shaderTable->GetGPUVirtualAddress() + m_shaderTableRecordSize;
	dispatchDesc.MissShaderTable.SizeInBytes = m_shaderTableRecordSize;
	dispatchDesc.MissShaderTable.StrideInBytes = m_shaderTableRecordSize;

	dispatchDesc.HitGroupTable.StartAddress = m_shaderTable->GetGPUVirtualAddress() + (m_shaderTableRecordSize * 2);
	dispatchDesc.HitGroupTable.SizeInBytes = m_shaderTableRecordSize;
	dispatchDesc.HitGroupTable.StrideInBytes = m_shaderTableRecordSize;

	dispatchDesc.Width = m_screenWidth;
	dispatchDesc.Height = m_screenHeight;
	dispatchDesc.Depth = 1;

	m_commandList->SetPipelineState1(m_rtStateObject);
	m_commandList->DispatchRays(&dispatchDesc);

	return true;
}

#pragma endregion