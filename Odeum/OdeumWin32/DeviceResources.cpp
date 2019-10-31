#include "DeviceResources.h"



DeviceResources::DeviceResources()
{
	m_device = 0;
	m_commandQueue = 0;
	m_swapChain = 0;
	m_renderTargetViewHeap = 0;
	m_backBufferRenderTarget[0] = 0;
	m_backBufferRenderTarget[1] = 0;
	m_backBufferRenderTarget[2] = 0;
	m_commandAllocator = 0;
	m_pipelineState = 0;
	m_fence = 0;
	m_fenceEvent = 0;
	m_currentFrame = 0;
	m_deviceRemoved = false;
	m_depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
}

DeviceResources::DeviceResources(const DeviceResources &)
{
}


DeviceResources::~DeviceResources()
{
}

// Went through initialize, all device resources are properly created and initialized
bool DeviceResources::Initialize(int screenHeight, int screenWidth, HWND hwnd, bool vsync, bool fullscreen)
{
	D3D_FEATURE_LEVEL					featureLevel;
	HRESULT								result;
	D3D12_COMMAND_QUEUE_DESC			commandQueueDesc;
	IDXGIFactory4*						factory;
	IDXGIAdapter1*						adapter;
	IDXGIOutput*						adapterOutput;
	unsigned int						numModes, i, numerator, denominator, renderTargetViewDescriptorSize;
	unsigned long long					stringLength;
	DXGI_MODE_DESC*						displayModeList;
	DXGI_ADAPTER_DESC					adapterDesc;
	int									error;
	DXGI_SWAP_CHAIN_DESC				swapChainDesc;
	IDXGISwapChain*						swapChain;
	D3D12_DESCRIPTOR_HEAP_DESC			renderTargetViewHeapDesc;
	D3D12_CPU_DESCRIPTOR_HANDLE			renderTargetViewHandle;

	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Set the feature level to DirectX 12.1 to enable using all the DirectX 12 features.
	// Note: Not all cards support full DirectX 12, this feature level may need to be reduced on some cards to 12.0.
	featureLevel = D3D_FEATURE_LEVEL_11_1; // --- Only supporting dx12, can change to include dx 11.1

	CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	for (UINT adapterIndex = 0; 
		DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter);
		adapterIndex++) {
		
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(adapter, featureLevel, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	// Check for DXR support
	result = IsDirectXRaytracingSupported(adapter);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"DirectX Raytracing is not supported by your OS, GPU and/or driver", L"Error", MB_OK);
		return false;
	}
	
	// TODO Aidan: Rewrite the adapter/output code to check for multiple adapters (i.e. integrated chip vs dedicated card)
	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Enumerate Outputs", L"Error", MB_OK);
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not get display mode list", L"Error", MB_OK);
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen height and width.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Height == (unsigned int)screenHeight)
		{
			if (displayModeList[i].Width == (unsigned int)screenWidth)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not get video card description", L"Error", MB_OK);
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	unsigned int m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Create the Direct3D 12 device.
	result = D3D12CreateDevice(adapter, 
		featureLevel, 
		IID_PPV_ARGS(&m_device));
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not create a DirectX 12.1 device.  The default video card does not support DirectX 12.1.", L"DirectX Device Failure", MB_OK);
		return false;
	}

	// Initialize the description of the command queue.
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	// Set up the description of the command queue.
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	// Create the command queue.
	result = m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(result))
	{
		return false;
	}

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set the swap chain to use double buffering.
	swapChainDesc.BufferCount = c_frameCount;

	// Set the height and width of the back buffers in the swap chain.
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Width = screenWidth;

	// Set a regular 32-bit surface for the back buffers.
	m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Format = m_backBufferFormat;

	// Set the usage of the back buffers to be render target outputs.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the swap effect to discard the previous buffer contents after swapping.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Finally create the swap chain using the swap chain description.	
	result = factory->CreateSwapChain(m_commandQueue, &swapChainDesc, &swapChain);
	if (FAILED(result))
	{
		return false;
	}

	// Next upgrade the IDXGISwapChain to a IDXGISwapChain3 interface and store it in a private member variable named m_swapChain.
	// This will allow us to use the newer functionality such as getting the current back buffer index.
	result = swapChain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&m_swapChain);
	if (FAILED(result))
	{
		return false;
	}

	// Clear pointer to original swap chain interface since we are using version 3 instead (m_swapChain).
	swapChain = 0;

	factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

	// Release the factory now that the swap chain has been created.
	factory->Release();
	factory = 0;

	// Initialize the render target view heap description for the two back buffers.
	ZeroMemory(&renderTargetViewHeapDesc, sizeof(renderTargetViewHeapDesc));

	// Set the number of descriptors to two for our two back buffers.  Also set the heap tyupe to render target views.
	renderTargetViewHeapDesc.NumDescriptors = c_frameCount;
	renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	renderTargetViewHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// Create the render target view heap for the back buffers.
	result = m_device->CreateDescriptorHeap(&renderTargetViewHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_renderTargetViewHeap);
	if (FAILED(result))
	{
		return false;
	}

	// Get a handle to the starting memory location in the render target view heap to identify where the render target views will be located for the two back buffers.
	renderTargetViewHandle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	// Get the size of the memory location for the render target view descriptors.
	renderTargetViewDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (int i = 0; i < c_frameCount; i++) {
		// Get a pointer to this back buffer from the swap chain.
		result = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBufferRenderTarget[i]));
		if (FAILED(result))
		{
			return false;
		}

		// Create a render target view for this back buffer.
		m_device->CreateRenderTargetView(m_backBufferRenderTarget[i], NULL, renderTargetViewHandle);

		// Increment the view handle to the next descriptor location in the render target view heap.
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	result = m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(result)) return false;

	D3D12_HEAP_PROPERTIES depthHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC depthResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_depthBufferFormat, screenWidth, screenHeight, 1, 1);
	depthResourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	CD3DX12_CLEAR_VALUE depthOptimizedClearValue(m_depthBufferFormat, 1.0f, 0);

	result = m_device->CreateCommittedResource(
		&depthHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&m_depthStencil));
	if (FAILED(result)) return false;


	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = m_depthBufferFormat;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	m_device->CreateDepthStencilView(m_depthStencil, &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a command allocator.
	result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&m_commandAllocator);
	if (FAILED(result))
	{
		return false;
	}

	// Create a fence for GPU synchronization.
	result = m_device->CreateFence(m_fenceValues[m_currentFrame], D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_fence);
	if (FAILED(result))
	{
		return false;
	}
	m_fenceValues[m_currentFrame]++;

	// Create an event object for the fence.
	m_fenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (m_fenceEvent == NULL)
	{
		return false;
	}

	// Initialize the starting fence value. 
	m_fenceValues[m_currentFrame]++;

	for (UINT n = 0; n < c_frameCount; n++) {
		m_fenceValues[n] = m_fenceValues[m_currentFrame];
	}

	// Set the 3D rendering viewport to target the entire window.
	m_viewPort = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f };

	return true;
}

void DeviceResources::InitializeFence()
{
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	m_fenceValue = 1;

	// Create an event handle to use for frame synchronization.
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// Wait for the command list to execute; we are reusing the same command 
	// list in our main loop but for now, we just want to wait for setup to 
	// complete before continuing.
	WaitForPrevFrame();
}

void DeviceResources::Uninitialize()
{
	int error;


	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	// Close the object handle to the fence event.
	error = CloseHandle(m_fenceEvent);
	if (error == 0)
	{
	}

	// Release the fence.
	if (m_fence)
	{
		m_fence->Release();
		m_fence = 0;
	}

	// Release the empty pipe line state.
	if (m_pipelineState)
	{
		m_pipelineState->Release();
		m_pipelineState = 0;
	}

	// Release the command allocator.
	if (m_commandAllocator)
	{
		m_commandAllocator->Release();
		m_commandAllocator = 0;
	}

	// Release the back buffer render target views.
	if (m_backBufferRenderTarget[0])
	{
		m_backBufferRenderTarget[0]->Release();
		m_backBufferRenderTarget[0] = 0;
	}
	if (m_backBufferRenderTarget[1])
	{
		m_backBufferRenderTarget[1]->Release();
		m_backBufferRenderTarget[1] = 0;
	}

	// Release the render target view heap.
	if (m_renderTargetViewHeap)
	{
		m_renderTargetViewHeap->Release();
		m_renderTargetViewHeap = 0;
	}

	// Release the swap chain.
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	// Release the command queue.
	if (m_commandQueue)
	{
		m_commandQueue->Release();
		m_commandQueue = 0;
	}

	// Release the device.
	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}
}

bool DeviceResources::Render()
{
	//HRESULT result;
	//const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];

	//// Finally present the back buffer to the screen since rendering is complete.
	//if (m_vsync_enabled)
	//{
	//	// Lock to screen refresh rate.
	//	result = m_swapChain->Present(1, 0);
	//	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	//	{
	//		m_deviceRemoved = true;
	//	}
	//}
	//else
	//{
	//	// Present as fast as possible.
	//	result = m_swapChain->Present(0, 0);
	//	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	//	{
	//		m_deviceRemoved = true;
	//		
	//	}
	//}

	//// Wait for Previous Frame() ----------------
	//// Signal and increment the fence value.
	//result = m_commandQueue->Signal(m_fence, m_fenceValues[m_currentFrame]);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	//// Wait until the GPU is done rendering.
	//if (m_fence->GetCompletedValue() < m_fenceValues[m_currentFrame])
	//{
	//	result = m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent);
	//	if (FAILED(result))
	//	{
	//		return false;
	//	}
	//	WaitForSingleObject(m_fenceEvent, INFINITE);
	//}

	//m_fenceValues[m_currentFrame] = currentFenceValue + 1;

	//return true;

	HRESULT result;
	result = m_swapChain->Present(1, 0);
	if (FAILED(result)) return false;
	if (!WaitForPrevFrame()) return false;

	return true;
}

bool DeviceResources::WaitForPrevFrame()
{
	HRESULT result;
	const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];
	result = m_commandQueue->Signal(m_fence, currentFenceValue);
	if (FAILED(result)) return false;
	
	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < currentFenceValue)
	{
		result = m_fence->SetEventOnCompletion(currentFenceValue, m_fenceEvent);
		if (FAILED(result)) return false;
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	m_fenceValues[m_currentFrame] = currentFenceValue + 1;

	return true;
}
