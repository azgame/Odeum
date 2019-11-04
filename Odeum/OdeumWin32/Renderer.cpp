#include "Renderer.h"
#include "DXRaytracingHelper.h"
#include "DXRHelper.h"
#include "DXR Includes/dxcapi.h"

const wchar_t* Renderer::c_hitGroupName = L"HitGroup";
const wchar_t* Renderer::c_raygenShaderName = L"MyRaygenShader";
const wchar_t* Renderer::c_closestHitShaderName = L"MyClosestHitShader";
const wchar_t* Renderer::c_missShaderName = L"MyMissShader";

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

bool Renderer::Initialize(int screenHeight, int screenWidth, HWND hwnd, std::vector<Model*> renderObjects)
{
	if (dxrEnabled) {
		if (!InitializeRaytrace(screenHeight, screenWidth, hwnd, renderObjects)) return false;
	}
	else {
		if (!InitializeRaster(screenHeight, screenWidth, hwnd, renderObjects)) return false;
	}

	return true;
}

bool Renderer::InitializeRaster(int screenHeight, int screenWidth, HWND hwnd, std::vector<Model*> renderObjects)
{
	HRESULT result;

	if (!InitializeDeviceResources(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN)) return false;

	m_device = m_deviceResources->GetD3Device();
	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	CD3DX12_DESCRIPTOR_RANGE range;
	CD3DX12_ROOT_PARAMETER parameter;

	range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	parameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);

	// Init Root Signature
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // Only the input assembler stage needs access to the constant buffer.
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
	descRootSignature.Init(1, &parameter, 0, nullptr, rootSignatureFlags);

	ID3DBlob* signature;
	ID3DBlob* error;

	D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	result = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	// Init Shaders
	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;

	result = D3DCompileFromFile(L"VertexShader.hlsl", NULL, NULL, "main", "vs_5_0", D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &vertexShader, NULL);
	if (FAILED(result)) return false;
	result = D3DCompileFromFile(L"PixelShader.hlsl", NULL, NULL, "main", "ps_5_0", D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &pixelShader, NULL);
	if (FAILED(result)) return false;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Init Pipeline Description
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_deviceResources->GetBackBufferFormat();
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	result = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(result)) return false;

	// Get the command list.
	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), m_pipelineState, IID_PPV_ARGS(&m_commandList));
	if (FAILED(result)) return false;

	// Initially we need to close the command list during initialization as it is created in a recording state.
	result = m_commandList->Close();
	if (FAILED(result)) return false;

	for (auto object : renderObjects) {
		object->Initialize(m_device, m_commandList);
	}

	CreateCBResources();

	m_deviceResources->InitializeFence();

	if (signature) signature->Release();
	if (error) error->Release();

	return true;
}

void Renderer::CreateRasterWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera)
{
	m_camera = camera;

	float aspectRatio = screenWidth / screenHeight;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_deviceResources->GetViewPort();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

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
	static const DirectX::XMFLOAT3 eye = { 0.0f, 0.7f, 1.5f };
	static const DirectX::XMFLOAT3 at = { 0.0f, -0.1f, 0.0f };
	static const DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

	m_camera->SetViewMatrix(eye, at, up);
	XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(m_camera->View()));
	XMStoreFloat4x4(&m_constantBufferData.model, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(0)));
}

bool Renderer::RenderRaster(std::vector<Model*> renderObjects)
{
	HRESULT								result;
	D3D12_RESOURCE_BARRIER				barrier;
	unsigned int						renderTargetViewDescriptorSize;
	float								color[4];

	XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(m_camera->View()));

	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	UINT8* destination = m_mappedConstantBuffer + (m_bufferIndex * c_alignedConstantBufferSize);
	memcpy(destination, &m_constantBufferData, sizeof(m_constantBufferData));

	// Reset (re-use) the memory associated command allocator.
	result = m_deviceResources->GetCommandAllocator()->Reset();
	if (FAILED(result)) return false;

	// Reset the command list
	result = m_commandList->Reset(m_deviceResources->GetCommandAllocator(), m_pipelineState);
	if (FAILED(result)) return false;

	m_commandList->SetGraphicsRootSignature(m_rootSignature);
	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// Bind the current frame's constant buffer to the pipeline.

	D3D12_VIEWPORT viewport = m_deviceResources->GetViewPort();
	m_commandList->RSSetViewports(1, &viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Record commands in the command list now.
	// Start by setting the resource barrier.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Get the render target view handle for the current back buffer.
	renderTargetViewDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_deviceResources->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(), m_bufferIndex, renderTargetViewDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvHeap->GetGPUDescriptorHandleForHeapStart(), m_bufferIndex, m_cbvHeapSize);
	m_commandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	// Then set the color to clear the window to.
	color[0] = 0.2;
	color[1] = 0.2;
	color[2] = 0.5;
	color[3] = 1.0;
	m_commandList->ClearRenderTargetView(rtvHandle, color, 0, NULL);
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_deviceResources->GetDepthStencilView();
	m_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the back buffer as the render target.
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &depthStencilView);

	// Populate the command list - i.e. pass the command list to the objects in the scene (as given to the renderer) 
	// and have the objects fill the command list with their resource data (buffer data)
	for (auto object : renderObjects) {
		object->Render(m_commandList);
	}

	// Indicate that the back buffer will now be used to present.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Close the list of commands.
	result = m_commandList->Close();
	if (FAILED(result)) return false;

	// Load the command list array (only one command list for now).
	ID3D12CommandList* ppCommandLists[] = { m_commandList };

	// Execute the list of commands.
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	if (!m_deviceResources->Render()) return false;

	return true;
}

#pragma endregion

#pragma region Common

bool Renderer::InitializeDeviceResources(int screenHeight, int screenWidth, HWND hwnd, bool VSYNC_ENABLED, bool FULL_SCREEN)
{
	HRESULT result;

	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	m_deviceResources = new DeviceResources(); // need to get device and commandlist from device resources
	if (!m_deviceResources) return false;
	bool dxsuccess = m_deviceResources->Initialize(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN);
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

	// Heap descriptors for constant buffer data
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = c_frameCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	// This flag indicates that this descriptor heap can be bound to the pipeline and that descriptors contained in it can be referenced by a root table
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeap));
	if (FAILED(result)) return false;

	// Init constant buffer
	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(c_frameCount * c_alignedConstantBufferSize);
	result = m_device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constantBuffer));
	if (FAILED(result)) return false;

	// Get memory location of constant buffers (one for each back buffer
	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
	m_cbvHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int n = 0; n < c_frameCount; n++)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = cbvGpuAddress;
		desc.SizeInBytes = c_alignedConstantBufferSize;
		m_device->CreateConstantBufferView(&desc, cbvCpuHandle);

		cbvGpuAddress += desc.SizeInBytes;
		cbvCpuHandle.Offset(m_cbvHeapSize);
	}

	// Map the constant buffers.
	CD3DX12_RANGE readRange(0, 0);
	result = m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer));
	if (FAILED(result)) return false;

	return true;
}

void Renderer::Uninitialize()
{
	if (m_commandList != nullptr)
	{
		m_commandList->Release();
		m_commandList = 0;
	}

	if (m_deviceResources != nullptr)
	{
		m_deviceResources->Uninitialize();
		delete m_deviceResources;
		m_deviceResources = nullptr;
	}
}

bool Renderer::Render(std::vector<Model*> renderObjects)
{
	if (dxrEnabled) {
		if (!RenderRaytrace(renderObjects)) return false;
	}
	else
	{
		if (!RenderRaster(renderObjects)) return false;
	}

	return true;
}

#pragma endregion

#pragma region DXR

bool Renderer::InitializeRaytrace(int screenHeight, int screenWidth, HWND hwnd, std::vector<Model*> renderObjects)
{
	m_raytracingOutputResourceUAVDescriptorHeapIndex = UINT_MAX;

	HRESULT result;

	// Create raytracing interfaces: raytracing device and commandlist.
	if (!CreateRaytracingInterfaces(screenHeight, screenWidth, hwnd)) return false;

	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&m_commandList));
	if (FAILED(result)) return false;

	for (auto object : renderObjects) {
		object->Initialize(m_device, m_commandList);
	}

	m_commandList->Close();

	// Build raytracing acceleration structures from the generated geometry.
	if (!BuildAccelerationStructures(renderObjects)) return false;

	// Create a raytracing pipeline state object which defines the binding of shaders, state and resources to be used during raytracing.
	if (!CreateRaytracingPipelineStateObject()) return false;

	// Create a heap for descriptors.
	if (!CreateDescriptorHeap(renderObjects)) return false;

	// Build shader tables, which define shaders and their local root arguments.
	if (!BuildShaderTables()) return false;

	// Create an output 2D texture to store the raytracing result to.
	//if (!CreateRaytracingOutputResource()) return false;

	return true;
}

bool Renderer::CreateRaytracingInterfaces(int screenHeight, int screenWidth, HWND hwnd)
{
	if (!InitializeDeviceResources(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN)) return false;

	m_device = m_deviceResources->GetD3Device();

	return true;
}

bool Renderer::BuildAccelerationStructures(std::vector<Model*> renderObjects)
{
	HRESULT result;

	m_commandList->Reset(m_deviceResources->GetCommandAllocator(), nullptr);

	D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
	geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	// TODO Aidan: For now we build one acceleration structure here for our one cube. 
	// Going forward each geometry builds its own structure, or belongs to a structure outside of the renderer
	// Hardcode object 0
	geometryDesc.Triangles.IndexBuffer = renderObjects[0]->GetIndexBuffer()->GetGPUVirtualAddress();
	geometryDesc.Triangles.IndexCount = static_cast<UINT>(renderObjects[0]->GetIndexCount());
	geometryDesc.Triangles.IndexFormat = renderObjects[0]->GetIndexBV().Format;
	geometryDesc.Triangles.Transform3x4 = 0;
	geometryDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	geometryDesc.Triangles.VertexCount = static_cast<UINT>(renderObjects[0]->GetVertexCount());
	geometryDesc.Triangles.VertexBuffer.StartAddress = renderObjects[0]->GetVertexBuffer()->GetGPUVirtualAddress();
	geometryDesc.Triangles.VertexBuffer.StrideInBytes = renderObjects[0]->GetVertexBV().StrideInBytes;

	// Mark the geometry as opaque. 
	// PERFORMANCE TIP: mark geometry as opaque whenever applicable as it can enable important ray processing optimizations.
	// Note: When rays encounter opaque geometry an any hit shader will not be executed whether it is present or not.
	geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

	// Get required sizes for an acceleration structure
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS topLevelInputs = {};
	topLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	topLevelInputs.Flags = buildFlags;
	topLevelInputs.NumDescs = 1;
	topLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO topLevelPrebuildInfo = {};
	m_device->GetRaytracingAccelerationStructurePrebuildInfo(&topLevelInputs, &topLevelPrebuildInfo);
	if (topLevelPrebuildInfo.ResultDataMaxSizeInBytes <= 0) return false;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelPrebuildInfo = {};
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS bottomLevelInputs = topLevelInputs;
	bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	bottomLevelInputs.pGeometryDescs = &geometryDesc;
	m_device->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &bottomLevelPrebuildInfo);
	if (bottomLevelPrebuildInfo.ResultDataMaxSizeInBytes <= 0) return false;

	ID3D12Resource* scratchResource;
	AllocateUAVBuffer(m_device, max(topLevelPrebuildInfo.ScratchDataSizeInBytes, bottomLevelPrebuildInfo.ScratchDataSizeInBytes), &scratchResource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"ScratchResource");

	// Allocate resources for acceleration structures.
	// Acceleration structures can only be placed in resources that are created in the default heap (or custom heap equivalent). 
	// Default heap is OK since the application doesnt need CPU read/write access to them. 
	// The resources that will contain acceleration structures must be created in the state D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, 
	// and must have resource flag D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS. The ALLOW_UNORDERED_ACCESS requirement simply acknowledges both: 
	//  - the system will be doing this type of access in its implementation of acceleration structure builds behind the scenes.
	//  - from the app point of view, synchronization of writes/reads to acceleration structures is accomplished using UAV barriers.
	D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;

	AllocateUAVBuffer(m_device, bottomLevelPrebuildInfo.ResultDataMaxSizeInBytes, &m_bottomLevelAccelerationStructure, initialResourceState, L"BottomLevelAccelerationStructure");
	AllocateUAVBuffer(m_device, topLevelPrebuildInfo.ResultDataMaxSizeInBytes, &m_topLevelAccelerationStructure, initialResourceState, L"TopLevelAccelerationStructure");

	// Create an instance desc for the bottom-level acceleration structure
	ID3D12Resource* instanceDescs;
	D3D12_RAYTRACING_INSTANCE_DESC instanceDesc = {};
	instanceDesc.Transform[0][0] = instanceDesc.Transform[1][1] = instanceDesc.Transform[2][2] = 1;
	instanceDesc.InstanceMask = 0xFF;
	instanceDesc.AccelerationStructure = m_bottomLevelAccelerationStructure->GetGPUVirtualAddress();
	AllocateUploadBuffer(m_device, &instanceDesc, sizeof(instanceDesc), &instanceDescs, L"InstanceDescs");

	// Bottom Level Acceleration Structure Desc
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC bottomLevelBuildDesc = {};
	bottomLevelBuildDesc.Inputs = bottomLevelInputs;
	bottomLevelBuildDesc.ScratchAccelerationStructureData = scratchResource->GetGPUVirtualAddress();
	bottomLevelBuildDesc.DestAccelerationStructureData = m_bottomLevelAccelerationStructure->GetGPUVirtualAddress();

	// Top Level Acceleration Structure Desc
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC topLevelBuildDesc = {};
	topLevelInputs.InstanceDescs = instanceDescs->GetGPUVirtualAddress();
	topLevelBuildDesc.Inputs = topLevelInputs;
	topLevelBuildDesc.DestAccelerationStructureData = m_topLevelAccelerationStructure->GetGPUVirtualAddress();
	topLevelBuildDesc.ScratchAccelerationStructureData = scratchResource->GetGPUVirtualAddress();

	auto BuildAccelerationStructure = [&](auto* raytracingCommandList)
	{
		raytracingCommandList->BuildRaytracingAccelerationStructure(&bottomLevelBuildDesc, 0, nullptr);
		raytracingCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::UAV(m_bottomLevelAccelerationStructure));
		raytracingCommandList->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);
	};

	// Build Acceleration structure
	BuildAccelerationStructure(m_commandList);

	// Kick off acceleration structure construction
	m_commandList->Close();
	/*ID3D12CommandList* commandLists[] = { m_commandList };
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(commandLists), commandLists);*/

	if (!m_deviceResources->WaitForPrevFrame()) return false;

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
	shaderCompiler.DxcDllHelper.Initialize();
	shaderCompiler.DxcDllHelper.CreateInstance(CLSID_DxcCompiler, &shaderCompiler.compiler);
	shaderCompiler.DxcDllHelper.CreateInstance(CLSID_DxcLibrary, &shaderCompiler.library);

	UINT32 code(0);
	IDxcBlobEncoding* pRaygen(nullptr);
	IDxcBlobEncoding* pHit(nullptr);
	IDxcBlobEncoding* pMiss(nullptr);

	DXRGlobal dxr;
	dxr.rgs = RtProgram(D3D12ShaderInfo(L"Raygen.hlsl", L"", L"lib_6_3"));
	dxr.miss = RtProgram(D3D12ShaderInfo(L"Miss.hlsl", L"", L"lib_6_3"));
	dxr.hit = HitProgram(L"Hit");
	dxr.hit.chs = RtProgram(D3D12ShaderInfo(L"Hit.hlsl", L"", L"lib_6_3"));

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

	D3D12_ROOT_PARAMETER rootParams[1] = { param0 };

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

	result = pResult->GetResult(&dxr.miss.blob);

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

	result = pResult->GetResult(&dxr.hit.chs.blob);

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
	shaderDesc.MaxAttributeSizeInBytes = D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES;

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

	// Create the RT Pipeline State Object (RTPSO)
	result = m_device->CreateStateObject(&pipelineDesc, IID_PPV_ARGS(&m_rtStateObject));
	if (FAILED(result)) return false;

	return true;
}

bool Renderer::CreateDescriptorHeap(std::vector<Model*> renderObjects)
{
	HRESULT result;

	CreateCBResources();

	// Describe the CBV/SRV/UAV heap
	// Need 7 entries:
	// 1 CBV for the ViewCB
	// 1 UAV for the RT output
	// 1 SRV for the Scene BVH
	// 1 SRV for the index buffer
	// 1 SRV for the vertex buffer
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 5;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// Create descriptor heap
	m_device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_descHeap));

	// Create the handle and get the heap size for increment
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_descHeap->GetCPUDescriptorHandleForHeapStart();
	m_descHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Create the constant buffer view description
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.SizeInBytes = ALIGN(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, sizeof(ViewCB));
	cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();

	m_device->CreateConstantBufferView(&cbvDesc, handle);

	// Create the dxr output buffer unordered access view
	DXGI_FORMAT backbufferFormat = m_deviceResources->GetBackBufferFormat();

	// Create the output resource. Dimensions and format should match swapChain
	CD3DX12_RESOURCE_DESC uavDesc = CD3DX12_RESOURCE_DESC::Tex2D(backbufferFormat, m_screenWidth, m_screenHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	CD3DX12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	result = m_device->CreateCommittedResource(
		&defaultHeapProperties,
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

	// Create the index buffer srv
	D3D12_SHADER_RESOURCE_VIEW_DESC indexSRVDesc;
	indexSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	indexSRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	indexSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
	indexSRVDesc.Buffer.StructureByteStride = 0;
	indexSRVDesc.Buffer.FirstElement = 0;
	indexSRVDesc.Buffer.NumElements = renderObjects[0]->GetIndexCount();
	indexSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	handle.ptr += m_descHeapSize;
	m_device->CreateShaderResourceView(renderObjects[0]->GetIndexBuffer(), &indexSRVDesc, handle);

	// Create the vertex buffer SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC vertexSRVDesc;
	vertexSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	vertexSRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	vertexSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
	vertexSRVDesc.Buffer.StructureByteStride = 0;
	vertexSRVDesc.Buffer.FirstElement = 0;
	vertexSRVDesc.Buffer.NumElements = (static_cast<UINT>(renderObjects[0]->GetVertexCount()) * sizeof(VertexType)) / sizeof(float);
	vertexSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	handle.ptr += m_descHeapSize;
	m_device->CreateShaderResourceView(renderObjects[0]->GetVertexBuffer(), &vertexSRVDesc, handle);

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

	ID3D12StateObjectProperties* stateObjectProperties;
	ThrowIfFailed(m_rtStateObject->QueryInterface(&stateObjectProperties));

	uint32_t shaderIdSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	uint32_t shaderTableSize = 0;

	m_shaderTableRecordSize = shaderIdSize;
	m_shaderTableRecordSize += 8;							// CBV/SRV/UAV descriptor table
	m_shaderTableRecordSize = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, m_shaderTableRecordSize);

	shaderTableSize = (m_shaderTableRecordSize * 3);		// 3 shader records in the table
	shaderTableSize = ALIGN(D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT, shaderTableSize);

	// Create the shader table buffer
	CD3DX12_RESOURCE_DESC m_shaderDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(shaderTableSize));

	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&m_shaderDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_shaderTable)
	);

	// Map the buffer
	uint8_t* pData;
	HRESULT hr = m_shaderTable->Map(0, nullptr, (void**)&pData);
	if (FAILED(hr)) return false;

	// Shader Record 0 - Ray Generation program and local root parameter data (descriptor table with constant buffer and IB/VB pointers)
	memcpy(pData, stateObjectProperties->GetShaderIdentifier(L"RayGen_12"), shaderIdSize);

	// Set the root parameter data. Point to start of descriptor heap.
	*reinterpret_cast<D3D12_GPU_DESCRIPTOR_HANDLE*>(pData + shaderIdSize) = m_descHeap->GetGPUDescriptorHandleForHeapStart();

	// Shader Record 1 - Miss program (no local root arguments to set)
	pData += m_shaderTableRecordSize;
	memcpy(pData, stateObjectProperties->GetShaderIdentifier(L"Miss_5"), shaderIdSize);

	// Shader Record 2 - Closest Hit program and local root parameter data (descriptor table with constant buffer and IB/VB pointers)
	pData += m_shaderTableRecordSize;
	memcpy(pData, stateObjectProperties->GetShaderIdentifier(L"HitGroup"), shaderIdSize);

	// Set the root parameter data. Point to start of descriptor heap.
	*reinterpret_cast<D3D12_GPU_DESCRIPTOR_HANDLE*>(pData + shaderIdSize) = m_descHeap->GetGPUDescriptorHandleForHeapStart();

	// Unmap
	m_shaderTable->Unmap(0, nullptr);

	//void* rayGenShaderIdentifier;
	//void* missShaderIdentifier;
	//void* hitGroupShaderIdentifier;

	//auto GetShaderIdentifiers = [&](auto* stateObjectProperties)
	//{
	//	rayGenShaderIdentifier = stateObjectProperties->GetShaderIdentifier(L"RayGen_12");
	//	missShaderIdentifier = stateObjectProperties->GetShaderIdentifier(L"Miss_5");
	//	hitGroupShaderIdentifier = stateObjectProperties->GetShaderIdentifier(L"HitGroup");
	//};

	//// Get shader identifiers.
	//UINT shaderIdentifierSize;
	//{
	//	ID3D12StateObjectProperties* stateObjectProperties;
	//	ThrowIfFailed(m_rtStateObject->QueryInterface(&stateObjectProperties));
	//	GetShaderIdentifiers(stateObjectProperties);
	//	shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	//}

	//// Ray gen shader table
	//{
	//	struct RootArguments {
	//		RayGenConstantBuffer cb;
	//	} rootArguments;
	//	rootArguments.cb = m_rayGenCB;

	//	UINT numShaderRecords = 1;
	//	UINT shaderRecordSize = shaderIdentifierSize + 8;
	//	ShaderTable rayGenShaderTable(m_device, numShaderRecords, shaderRecordSize, L"RayGenShaderTable");
	//	rayGenShaderTable.push_back(ShaderRecord(rayGenShaderIdentifier, shaderIdentifierSize, &rootArguments, sizeof(rootArguments)));
	//	m_rayGenShaderTable = rayGenShaderTable.GetResource().Get();
	//}

	//// Miss shader table
	//{
	//	UINT numShaderRecords = 1;
	//	UINT shaderRecordSize = shaderIdentifierSize;
	//	ShaderTable missShaderTable(m_device, numShaderRecords, shaderRecordSize, L"MissShaderTable");
	//	missShaderTable.push_back(ShaderRecord(missShaderIdentifier, shaderIdentifierSize));
	//	m_missShaderTable = missShaderTable.GetResource().Get();
	//}

	//// Hit group shader table
	//{
	//	UINT numShaderRecords = 1;
	//	UINT shaderRecordSize = shaderIdentifierSize;
	//	ShaderTable hitGroupShaderTable(m_device, numShaderRecords, shaderRecordSize, L"HitGroupShaderTable");
	//	hitGroupShaderTable.push_back(ShaderRecord(hitGroupShaderIdentifier, shaderIdentifierSize));
	//	m_hitGroupShaderTable = hitGroupShaderTable.GetResource().Get();
	//}

	return true;
}

bool Renderer::RenderRaytrace(std::vector<Model*> renderObjects)
{
	HRESULT result;

	// Reset command list and allocator.
	result = m_deviceResources->GetCommandAllocator()->Reset();
	if (FAILED(result)) return false;
	ThrowIfFailed(m_commandList->Reset(m_deviceResources->GetCommandAllocator(), nullptr));

	XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(m_camera->View()));

	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	UINT8* destination = m_mappedConstantBuffer + (m_bufferIndex * c_alignedConstantBufferSize);
	memcpy(destination, &m_constantBufferData, sizeof(m_constantBufferData));

	// Transition the render target into the correct state to allow for drawing into it.
	//m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	DoRaytracing();
	CopyRaytracingOutputToBackbuffer();

	//m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Execute command lists
	m_commandList->Close();
	ID3D12CommandList* commandLists[] = { m_commandList };
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(commandLists), commandLists);

	return true;
}

bool Renderer::DoRaytracing()
{
	//m_commandList->SetComputeRootSignature(m_raytracingGlobalRootSignature);

	// Bind the heaps, acceleration structure and dispatch rays
	ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	D3D12_DISPATCH_RAYS_DESC dispatchDesc = {};

	//m_commandList->SetComputeRootDescriptorTable(GlobalRootSignatureParams::OutputViewSlot, m_raytracingOutputResourceUAVGpuDescriptor);
	//m_commandList->SetComputeRootShaderResourceView(GlobalRootSignatureParams::AccelerationStructureSlot, m_topLevelAccelerationStructure->GetGPUVirtualAddress());

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

bool Renderer::CopyRaytracingOutputToBackbuffer()
{
	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER preCopyBarriers[2];
	preCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
	preCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_raytracingOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);

	m_commandList->ResourceBarrier(_countof(preCopyBarriers), preCopyBarriers);
	m_commandList->CopyResource(m_deviceResources->GetBackBuffer(m_bufferIndex), m_raytracingOutput);

	D3D12_RESOURCE_BARRIER postCopyBarriers[2];
	postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
	postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_raytracingOutput, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	m_commandList->ResourceBarrier(_countof(postCopyBarriers), postCopyBarriers);

	return true;
}

UINT Renderer::AllocateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE * cpuDescriptor, UINT descriptorIndexToUse)
{
	auto descriptorHeapCpuBase = m_descHeap->GetCPUDescriptorHandleForHeapStart();
	if (descriptorIndexToUse >= m_descHeap->GetDesc().NumDescriptors)
	{
		descriptorIndexToUse = m_descHeapAllocated++;
	}
	*cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeapCpuBase, descriptorIndexToUse, m_descHeapSize);
	return descriptorIndexToUse;
}

bool Renderer::CreateRaytracingOutputResource()
{
	HRESULT result;

	DXGI_FORMAT backbufferFormat = m_deviceResources->GetBackBufferFormat();

	// Create the output resource. Dimensions and format should match swapChain
	CD3DX12_RESOURCE_DESC uavDesc = CD3DX12_RESOURCE_DESC::Tex2D(backbufferFormat, m_screenWidth, m_screenHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	CD3DX12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	result = m_device->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&uavDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&m_raytracingOutput)
	);

	if (FAILED(result)) return false;

	D3D12_CPU_DESCRIPTOR_HANDLE uavDescriptorHandle;
	m_raytracingOutputResourceUAVDescriptorHeapIndex = AllocateDescriptor(&uavDescriptorHandle, m_raytracingOutputResourceUAVDescriptorHeapIndex);
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	m_device->CreateUnorderedAccessView(m_raytracingOutput, nullptr, &UAVDesc, uavDescriptorHandle);

	return true;
}

#pragma endregion


