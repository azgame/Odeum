#include "Renderer.h"



Renderer::Renderer()
{
	m_deviceResources = nullptr;
	m_triangle = new Model();
}

Renderer::Renderer(const Renderer& other)
{
}


Renderer::~Renderer()
{
}

bool Renderer::Initialize(int screenHeight, int screenWidth, HWND hwnd)
{
	HRESULT result;
	
	m_deviceResources = new DeviceResources(); // need to get device and commandlist from device resources
	if (!m_deviceResources) return false;
	bool dxsuccess = m_deviceResources->Initialize(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN);
	if (!dxsuccess)
	{
		MessageBox(hwnd, L"Could not initialize DirectX", L"Error", MB_OK);
		return false;
	}

	m_device = m_deviceResources->GetD3Device();
	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	CD3DX12_DESCRIPTOR_RANGE range;
	CD3DX12_ROOT_PARAMETER parameter;

	range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	parameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);

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
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
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

	m_triangle->Initialize(m_device, m_commandList);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = c_frameCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	// This flag indicates that this descriptor heap can be bound to the pipeline and that descriptors contained in it can be referenced by a root table.
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeap));
	if (FAILED(result)) return false;

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


	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
	m_cbvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int n = 0; n < c_frameCount; n++)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = cbvGpuAddress;
		desc.SizeInBytes = c_alignedConstantBufferSize;
		m_device->CreateConstantBufferView(&desc, cbvCpuHandle);

		cbvGpuAddress += desc.SizeInBytes;
		cbvCpuHandle.Offset(m_cbvDescriptorSize);
	}

	// Map the constant buffers.
	CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
	result = m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer));
	if (FAILED(result)) return false;

	CreateWindowSizeDependentResources(screenHeight, screenWidth);

	m_deviceResources->InitializeFence();

	if (signature) signature->Release();
	if (error) error->Release();

	return true;
}

void Renderer::CreateWindowSizeDependentResources(int screenHeight, int screenWidth)
{
	float aspectRatio = screenWidth / screenHeight;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_deviceResources->GetViewPort();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		DirectX::XMMatrixTranspose(perspectiveMatrix)
	);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const DirectX::XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, at, up)));
	// Prepare to pass the updated model matrix to the shader.
	XMStoreFloat4x4(&m_constantBufferData.model, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(0)));
}

void Renderer::Uninitialize()
{
	if (m_commandList)
	{
		m_commandList->Release();
		m_commandList = 0;
	}
	
	if (m_deviceResources)
	{
		m_deviceResources->Uninitialize();
		delete m_deviceResources;
		m_deviceResources = nullptr;
	}
}

bool Renderer::Frame()
{
	if (!Render()) return false;
	
	return true;
}

bool Renderer::Render()
{
	HRESULT								result;
	D3D12_RESOURCE_BARRIER				barrier;
	unsigned int						renderTargetViewDescriptorSize;
	float								color[4];

	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	UINT8* destination = m_mappedConstantBuffer + (m_bufferIndex * c_alignedConstantBufferSize);
	memcpy(destination, &m_constantBufferData, sizeof(m_constantBufferData));

	// Reset (re-use) the memory associated command allocator.
	result = m_deviceResources->GetCommandAllocator()->Reset();
	if (FAILED(result))
	{
		return false;
	}

	// Reset the command list, use empty pipeline state for now since there are no shaders and we are just clearing the screen.
	result = m_commandList->Reset(m_deviceResources->GetCommandAllocator(), m_pipelineState);
	if (FAILED(result))
	{
		return false;
	}

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

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvHeap->GetGPUDescriptorHandleForHeapStart(), m_bufferIndex, m_cbvDescriptorSize);
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
	m_triangle->Render(m_commandList);

	// Indicate that the back buffer will now be used to present.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetBackBuffer(m_bufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Close the list of commands.
	result = m_commandList->Close();
	if (FAILED(result))
	{
		return false;
	}

	// Load the command list array (only one command list for now).
	ID3D12CommandList* ppCommandLists[] = { m_commandList };

	// Execute the list of commands.
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
	if (!m_deviceResources->Render()) return false;
	
	return true;
}
