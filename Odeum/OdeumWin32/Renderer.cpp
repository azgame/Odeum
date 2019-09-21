#include "Renderer.h"



Renderer::Renderer()
{
	m_deviceResources = nullptr;
	m_triangle = nullptr;
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

	/*result = D3DCompileFromFile(L"Color.vs", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "vs_5_0", 0, 0, &vertexShader, NULL);
	if (!result) return false;
	result = D3DCompileFromFile(L"Color.ps", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "ps_5_0", 0, 0, &pixelShader, NULL);
	if (!result) return false;*/
		
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature;
	/*psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };*/
	psoDesc.RasterizerState = D3D12_RASTERIZER_DESC();
	psoDesc.BlendState = D3D12_BLEND_DESC();
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	
	result = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (!result) return false;

	// Create a basic command list.
	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&m_commandList);
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

	if (signature) signature->Release();
	if (error) error->Release();
	/*if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();*/

	return true;
}

void Renderer::Uninitialize()
{
	if (m_deviceResources)
	{
		m_deviceResources->Uninitialize();
		delete m_deviceResources;
		m_deviceResources = nullptr;
	}

	if (m_commandList)
	{
		m_commandList->Release();
		m_commandList = 0;
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
	ID3D12CommandList*					ppCommandLists[1];
	unsigned int						m_bufferIndex;

	m_bufferIndex = m_deviceResources->GetSwapChain()->GetCurrentBackBufferIndex();

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

	// Set the back buffer as the render target.
	m_commandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, NULL);

	// Then set the color to clear the window to.
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1.0;
	m_commandList->ClearRenderTargetView(renderTargetViewHandle, color, 0, NULL);

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
	ppCommandLists[0] = m_commandList;

	// Execute the list of commands.
	m_deviceResources->GetCommandQ()->ExecuteCommandLists(1, ppCommandLists);
	
	if (!m_deviceResources->Render()) return false;

	// Alternate the back buffer index back and forth between 0 and 1 each frame.
	m_bufferIndex == 0 ? m_bufferIndex = 1 : m_bufferIndex = 0;
	
	return true;
}
