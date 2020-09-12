#include "D3DCore.h"

#include "DXCommandContext.h"
#include "CommandListManager.h"

#include "Buffers/ColourBuffer.h"
#include "Buffers/DepthBuffer.h"

#include "../../Core/OdeumEngine.h"



namespace DXGraphics
{
	ID3D12Device* m_device = nullptr;
	CommandListManager m_commandManager;
	ContextManager m_contextManager;

	DescriptorAllocator m_descriptorAllocators[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
	{
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
	};

	float s_frameTime = 0.0f;
	uint64_t s_frameIndex = 0;
	int64_t s_frameStartTick = 0;
	bool s_ultraWide = false;
	float ultraWideRatio = 1.34375f;

	uint32_t m_nativeWidth = 0;
	uint32_t m_nativeHeight = 0;
	uint32_t m_displayWidth = 0;
	uint32_t m_displayHeight = 0;

	bool s_enableVSync = false;

	ResolutionOptions m_targetResolution = k900p;

	D3D_FEATURE_LEVEL g_D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_FORMAT swapChainFormat = DXGI_FORMAT_R10G10B10A2_UNORM;

	UINT m_currentBuffer = 0;
	
	void SetNativeResolution()
	{
		uint32_t nativeWidth, nativeHeight;

		float ultraWideExpansion = s_ultraWide ? ultraWideRatio : 1.0f;

		switch (ResolutionOptions((int)m_targetResolution))
		{
		default:
		case k720p:
			nativeWidth = (int)((float)1280 * ultraWideExpansion);
			nativeHeight = 720;
			break;
		case k900p:
			nativeWidth = (int)((float)1600 * ultraWideExpansion);
			nativeHeight = 900;
			break;
		case k1080p:
			nativeWidth = (int)((float)1920 * ultraWideExpansion);
			nativeHeight = 1080;
			break;
		case k1440p:
			nativeWidth = (int)((float)2560 * ultraWideExpansion);
			nativeHeight = 1440;
			break;
		case k1800p:
			nativeWidth = (int)((float)3200 * ultraWideExpansion);;
			nativeHeight = 1800;
			break;
		case k2160p:
			nativeWidth = (int)((float)3840 * ultraWideExpansion);;
			nativeHeight = 2160;
			break;
		}

		if (m_nativeWidth == nativeWidth && m_nativeHeight == nativeHeight)
			return;

		m_nativeWidth = nativeWidth;
		m_nativeHeight = nativeHeight;

		m_commandManager.IdleGPU();
		InitializeRenderingBuffers(nativeWidth, nativeHeight);
	}

	ColourBuffer m_displayPlane[SWAP_CHAIN_BUFFER_COUNT];
	ColourBuffer m_preDisplayBuffer;
	ColourBuffer m_presentBuffer;
	DepthBuffer m_sceneDepthBuffer;

	IDXGISwapChain1* sm_swapChain = nullptr;

	RootSignature m_presentRootSig;
	GraphicsPSO m_presentPSO;

	D3D12_BLEND_DESC alphaBlend;
	D3D12_RASTERIZER_DESC rasterDesc;
	D3D12_DEPTH_STENCIL_DESC depthStateDisabled;

	void PreparePresent();
	void InitializeCommonState();
}

void DXGraphics::Initialize()
{

#if _DEBUG
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#else
	Debug::Log("Did not create Dx12 debug validation layer", __FILENAME__, __LINE__);
#endif
	
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;

	CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	size_t maxSize = 0;

	for (UINT adapterIndex = 0;
		DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter);
		adapterIndex++)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags && DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (desc.DedicatedVideoMemory > maxSize && SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
		{
			adapter->GetDesc1(&desc);
			maxSize = desc.DedicatedVideoMemory;
		}
	}

	m_displayWidth = OdeumEngine::Get().GetWindow().GetWidth();
	m_displayHeight = OdeumEngine::Get().GetWindow().GetHeight();
	s_ultraWide = OdeumEngine::Get().GetWindow().GetUltraWide();
	m_displayWidth = s_ultraWide ? m_displayWidth * ultraWideRatio : m_displayWidth;

	m_commandManager.Initialize(m_device);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = m_displayWidth;
	swapChainDesc.Height = m_displayHeight;
	swapChainDesc.Format = swapChainFormat;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	if (FAILED(factory->CreateSwapChainForHwnd(m_commandManager.GetCommandQueue(), OdeumEngine::Get().GetWindow().GetHWND(),
		&swapChainDesc, nullptr, nullptr, &sm_swapChain)))
		Debug::FatalError("Could not create swap chain!", __FILENAME__, __LINE__);

	ID3D12Resource* tempDisplayPlane[3];
	
	for (uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		if (FAILED(sm_swapChain->GetBuffer(i, IID_PPV_ARGS(&tempDisplayPlane[i]))))
			Debug::Error("Could not retrieve swap chain buffer " + i, __FILENAME__, __LINE__);
		m_displayPlane[i].CreateFromSwapChain(L"Primary swap chain buffer", tempDisplayPlane[i]);
	}

	tempDisplayPlane[0]->Release();
	tempDisplayPlane[1]->Release();
	tempDisplayPlane[2]->Release();

	InitializeCommonState();

	m_presentRootSig.Reset(1, 0);
	m_presentRootSig[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_presentRootSig.Finalize(L"Present");

	m_presentPSO.SetRootSignature(m_presentRootSig);
	m_presentPSO.SetRasterizerState(rasterDesc);
	m_presentPSO.SetBlendState(alphaBlend);
	m_presentPSO.SetDepthStencilState(depthStateDisabled);
	m_presentPSO.SetSampleMask(0xFFFFFFFF);
	m_presentPSO.SetInputLayout(0, nullptr);
	m_presentPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_presentPSO.CompileVertexShader(L"Engine/Shaders/ScreenQuad.hlsl", "main", "vs_5_0");
	m_presentPSO.CompilePixelShader(L"Engine/Shaders/Present.hlsl", "main", "ps_5_0");
	m_presentPSO.SetRenderTargetFormat(swapChainFormat, DXGI_FORMAT_UNKNOWN);
	m_presentPSO.Finalize();

	InitializeRenderingBuffers(m_displayWidth, m_displayHeight);
}

void DXGraphics::InitializeRenderingBuffers(uint32_t nativeWidth_, uint32_t nativeHeight_)
{
	GraphicsContext& initBuffers = GraphicsContext::RequestContext();

	m_preDisplayBuffer.Create(L"Pre display buffer", nativeWidth_, nativeHeight_, 1, swapChainFormat);
	m_sceneDepthBuffer.Create(L"Scene depth buffer", nativeWidth_, nativeHeight_, DXGI_FORMAT_D32_FLOAT);
	m_presentBuffer.Create(L"Present buffer", nativeWidth_, nativeHeight_, 1, DXGI_FORMAT_R11G11B10_FLOAT);

	m_presentBuffer.SetClearColour(Colour(0.20f, 1.0f, 1.0f));

	initBuffers.Finish();
}

void DXGraphics::Resize(uint32_t width_, uint32_t height_)
{
	ASSERT(sm_swapChain != nullptr, "Swap chain has not been initialized");

	if (width_ == 0 || height_ == 0)
		return;

	if (width_ == m_displayWidth && height_ == m_displayHeight)
		return;

	m_commandManager.IdleGPU();

	m_displayWidth = width_;
	m_displayHeight = height_;

	m_preDisplayBuffer.Create(L"Pre display buffer", width_, height_, 1, swapChainFormat);

	for (uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		m_displayPlane[i].Destroy();
	}

	if (FAILED(sm_swapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, width_, height_, swapChainFormat, 0)))
		Debug::Error("Could not resize buffers!", __FILENAME__, __LINE__);

	ID3D12Resource* tempDisplayPlane[3];

	for (uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		if (FAILED(sm_swapChain->GetBuffer(i, IID_PPV_ARGS(&tempDisplayPlane[i]))))
			Debug::Error("Could not retrieve swap chain buffer " + i, __FILENAME__, __LINE__);
		
		m_displayPlane[i].CreateFromSwapChain(L"Primary swap chain buffer", tempDisplayPlane[i]);
		m_displayPlane[i].SetClearColour(Colour(1.0f, 0.33f, 0.67f));
	}

	tempDisplayPlane[0]->Release();
	tempDisplayPlane[1]->Release();
	tempDisplayPlane[2]->Release();

	m_currentBuffer = 0;
	m_commandManager.IdleGPU();

	// ResizeDisplayDependentBuffers(m_nativeWidth, m_nativeHeight);
}

void DXGraphics::PreparePresent()
{
	GraphicsContext& context = GraphicsContext::RequestContext(L"Present");

	context.TransitionResource(m_presentBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	context.TransitionResource(m_displayPlane[m_currentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET);

	context.SetRootSignature(m_presentRootSig);
	context.SetDynamicDescriptor(0, 0, m_presentBuffer.GetSRV());

	context.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvs[] =
	{
		m_displayPlane[m_currentBuffer].GetRTV()
	};

	context.SetPipelineState(m_presentPSO);
	context.SetRenderTargets(_countof(rtvs), rtvs);
	context.SetViewportAndScissor(0, 0, m_displayWidth, m_displayHeight);

	context.Draw(3);

	context.TransitionResource(m_displayPlane[m_currentBuffer], D3D12_RESOURCE_STATE_PRESENT);

	context.Finish();
}

void DXGraphics::Present()
{
	PreparePresent();

	m_currentBuffer = (m_currentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

	UINT presentInterval = s_enableVSync ? min(4, (int)round(s_frameTime * 100.0f)) : 0;

	sm_swapChain->Present(presentInterval, 0);

	if (s_enableVSync)
		s_frameTime = 100.0f;
	else
		s_frameTime = OdeumEngine::Get().GetTimer().GetDeltaTime();

	SetNativeResolution();
}

void DXGraphics::InitializeCommonState()
{
	alphaBlend = {};
	alphaBlend.IndependentBlendEnable = FALSE;
	alphaBlend.RenderTarget[0].BlendEnable = FALSE;
	alphaBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	alphaBlend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	alphaBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	alphaBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	alphaBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	alphaBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	alphaBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	depthStateDisabled;
	depthStateDisabled.DepthEnable = FALSE;
	depthStateDisabled.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStateDisabled.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStateDisabled.StencilEnable = FALSE;
	depthStateDisabled.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStateDisabled.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthStateDisabled.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStateDisabled.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthStateDisabled.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStateDisabled.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStateDisabled.BackFace = depthStateDisabled.FrontFace;

	rasterDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
}

void DXGraphics::Shutdown()
{
	m_commandManager.IdleGPU();
	sm_swapChain->SetFullscreenState(false, nullptr);

	CommandContext::DestroyAllContexts();
	m_commandManager.Uninitialize();
	sm_swapChain->Release();

	DescriptorAllocator::DestroyHeaps();

	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_displayPlane[i].Destroy();

	m_preDisplayBuffer.Destroy();
	m_presentBuffer.Destroy();
	m_sceneDepthBuffer.Destroy();

	if (m_device != nullptr) m_device->Release(); m_device = nullptr;
}

uint64_t DXGraphics::GetFrameCount()
{
	return s_frameIndex;
}

float DXGraphics::GetFrameTime()
{
	return s_frameTime;
}

float DXGraphics::GetFrameRate()
{
	return s_frameTime == 0.0f ? 0.0f : 1.0f / s_frameTime;
}