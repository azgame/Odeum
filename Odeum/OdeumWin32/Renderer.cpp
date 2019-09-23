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
	result = m_deviceResources->Initialize(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX", L"Error", MB_OK);
		return false;
	}

	m_device = m_deviceResources->GetD3Device();
	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.NumParameters = 0; //(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	rootSignatureDesc.pParameters = nullptr; 
	rootSignatureDesc.NumStaticSamplers = 0; 
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature;
	ID3DBlob* error;

	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	result = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;

	result = D3DCompileFromFile(L"VertexShader.hlsl", NULL, NULL, "main", "vs_5_0", 0, 0, &vertexShader, NULL);
	if (FAILED(result)) return false;
	result = D3DCompileFromFile(L"PixelShader.hlsl", NULL, NULL, "main", "ps_5_0", 0, 0, &pixelShader, NULL);
	if (FAILED(result)) return false;
		
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_RASTERIZER_DESC rDesc;
	rDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rDesc.CullMode = D3D12_CULL_MODE_BACK;
	rDesc.FrontCounterClockwise = FALSE;
	rDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rDesc.DepthClipEnable = TRUE;
	rDesc.MultisampleEnable = FALSE;
	rDesc.AntialiasedLineEnable = FALSE;
	rDesc.ForcedSampleCount = 0;
	rDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_BLEND_DESC bDesc;
	bDesc.AlphaToCoverageEnable = FALSE;
	bDesc.IndependentBlendEnable = FALSE;
	bDesc.RenderTarget[0].BlendEnable = FALSE;
	bDesc.RenderTarget[0].LogicOpEnable = FALSE;
	bDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	bDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	bDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	bDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	bDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	bDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	bDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	bDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
	psoDesc.RasterizerState = rDesc;
	psoDesc.BlendState = bDesc;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	
	result = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(result)) return false;

	// Get the command list.
	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), m_pipelineState, IID_PPV_ARGS(&m_commandList));
	if (FAILED(result))
	{
		return false;
	}

	// Initially we need to close the command list during initialization as it is created in a recording state.
	result = m_commandList->Close();
	if (FAILED(result))
	{
		return false;
	}

	m_triangle->Initialize(m_device, m_commandList);

	if (signature) signature->Release();
	if (error) error->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();

	return true;
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
	D3D12_CPU_DESCRIPTOR_HANDLE			renderTargetViewHandle;
	unsigned int						renderTargetViewDescriptorSize;
	float								color[4];

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

	// Record commands in the command list now.
	// Start by setting the resource barrier.
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_deviceResources->GetBackBuffer(m_bufferIndex);
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_commandList->ResourceBarrier(1, &barrier);

	// Get the render target view handle for the current back buffer.
	renderTargetViewHandle = m_deviceResources->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (m_bufferIndex == 1)
	{
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}

	// Then set the color to clear the window to.
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1.0;
	m_commandList->ClearRenderTargetView(renderTargetViewHandle, color, 0, NULL);

	// Set the back buffer as the render target.
	m_commandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, NULL);

	// Populate the command list - i.e. pass the command list to the objects in the scene (as given to the renderer) 
	// and have the objects fill the command list with their resource data (buffer data)
	m_triangle->Render(m_commandList);

	// Indicate that the back buffer will now be used to present.
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_commandList->ResourceBarrier(1, &barrier);

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

	// Alternate the back buffer index back and forth between 0 and 1 each frame.
	m_bufferIndex == 0 ? m_bufferIndex = 1 : m_bufferIndex = 0;
	
	return true;
}
