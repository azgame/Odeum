#include "D3DCore.h"

#include "CommandContext.h"
#include "CommandListManager.h"

#include "Buffers/ColourBuffer.h"
#include "Buffers/DepthBuffer.h"
#include "CommandSignature.h"

#include "../../Core/OdeumEngine.h"

#include "../Common/GraphicsCore.h"

#include "TextureManager.h"

#include <algorithm>

using namespace Graphics;

// replace with getting refresh rate from monitor in initialization
#define REFRESH_RATE 100.0f
#define NUM_FRAMES_FOR_AVERAGES 60

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

	float ultraWideRatio = 1.34375f;

	uint32_t m_nativeWidth = 0;
	uint32_t m_nativeHeight = 0;

	D3D_FEATURE_LEVEL g_D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_FORMAT swapChainFormat = DXGI_FORMAT_R10G10B10A2_UNORM;

	void PreparePresent();
	void InitializeCommonState();
	void InitializeRenderingBuffers(uint32_t nativeWidth_, uint32_t nativeHeight_);
	
	void SetNativeResolution()
	{
		uint32_t nativeWidth, nativeHeight;

		float ultraWideExpansion = s_ultraWide ? ultraWideRatio : 1.0f;

		switch (ResolutionOptions((int)s_targetResolution))
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
			nativeWidth = (int)((float)3200 * ultraWideExpansion);
			nativeHeight = 1800;
			break;
		case k2160p:
			nativeWidth = (int)((float)3840 * ultraWideExpansion);
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
	ColourBuffer m_overlayBuffer;
	DepthBuffer m_sceneDepthBuffer;

	CommandSignature DispatchIndirectCommandSignature(1);
	CommandSignature DrawIndirectCommandSignature(1);

	IDXGISwapChain1* sm_swapChain = nullptr;

	RootSignature m_presentRootSig;
	GraphicsPSO m_presentPSO;

	D3D12_BLEND_DESC alphaBlend;
	D3D12_BLEND_DESC blendPreMultiplied;
	D3D12_RASTERIZER_DESC rasterDesc;
	D3D12_RASTERIZER_DESC rasterTwoSided;
	D3D12_DEPTH_STENCIL_DESC depthReadWrite;
	D3D12_DEPTH_STENCIL_DESC depthReadOnly;

	float frameTimeAverage = 0.0f;
	float frameTimeTotal = 0.0f;
	uint32_t frameCounter = 0;
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

		if (desc.DedicatedVideoMemory > maxSize && SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device))))
		{
			adapter->GetDesc1(&desc);
			maxSize = desc.DedicatedVideoMemory;
		}
	}

	s_displayWidth = OdeumEngine::Get().GetWindow().GetWidth();
	s_displayHeight = OdeumEngine::Get().GetWindow().GetHeight();
	s_ultraWide = OdeumEngine::Get().GetWindow().isUltraWide();
	s_displayWidth = s_ultraWide ? (uint32_t)((float)s_displayWidth * ultraWideRatio) : s_displayWidth;
	s_enableVSync = false;

	m_commandManager.Initialize(m_device);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = s_displayWidth;
	swapChainDesc.Height = s_displayHeight;
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
	
	for (uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> tempDisplayPlane;
		if (FAILED(sm_swapChain->GetBuffer(i, IID_PPV_ARGS(&tempDisplayPlane))))
			Debug::Error("Could not retrieve swap chain buffer " + i, __FILENAME__, __LINE__);
		m_displayPlane[i].CreateFromSwapChain(L"Primary swap chain buffer", tempDisplayPlane.Detach());
	}

	InitializeCommonState();

	m_presentRootSig.Reset(1, 0);
	m_presentRootSig[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 2, D3D12_SHADER_VISIBILITY_PIXEL);
	m_presentRootSig.Finalize(L"Present");

	m_presentPSO.SetRootSignature(m_presentRootSig);
	m_presentPSO.SetRasterizerState(rasterDesc);
	rasterDesc.CullMode = D3D12_CULL_MODE_BACK;

	m_presentPSO.SetBlendState(alphaBlend);

	m_presentPSO.SetDepthStencilState(depthReadWrite);
	depthReadWrite.DepthEnable = TRUE;
	depthReadWrite.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthReadWrite.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	m_presentPSO.SetSampleMask(0xFFFFFFFF);
	m_presentPSO.SetInputLayout(0, nullptr);
	m_presentPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_presentPSO.CompileVertexShader(L"Engine/Shaders/ScreenQuad.hlsl", "main", "vs_5_0");
	m_presentPSO.CompilePixelShader(L"Engine/Shaders/Present.hlsl", "main", "ps_5_0");
	m_presentPSO.SetRenderTargetFormat(swapChainFormat, DXGI_FORMAT_UNKNOWN);
	m_presentPSO.Finalize();

	InitializeRenderingBuffers(s_displayWidth, s_displayHeight);

	TextureManager::Get()->Initialize("Engine/Resources/Textures/");
}

void DXGraphics::InitializeRenderingBuffers(uint32_t nativeWidth_, uint32_t nativeHeight_)
{
	GraphicsContext& initBuffers = GraphicsContext::RequestContext();

	m_preDisplayBuffer.Create(L"Pre display buffer", nativeWidth_, nativeHeight_, 1, swapChainFormat);
	m_sceneDepthBuffer.Create(L"Scene depth buffer", nativeWidth_, nativeHeight_, DXGI_FORMAT_D32_FLOAT);
	m_presentBuffer.SetClearColour(Colour(0.20f, 1.0f, 1.0f));
	m_presentBuffer.Create(L"Present buffer", nativeWidth_, nativeHeight_, 1, DXGI_FORMAT_R11G11B10_FLOAT);
	m_overlayBuffer.SetClearColour(Colour(1.0f, 1.0f, 1.0f, 0.0f));
	m_overlayBuffer.Create(L"Overlay buffer", nativeWidth_, nativeHeight_, 1, DXGI_FORMAT_R8G8B8A8_UNORM);

	initBuffers.Finish();
}

void DXGraphics::Resize(uint32_t width_, uint32_t height_)
{
	if (sm_swapChain == nullptr)
	{
		return;
	}

	if (width_ == 0 || height_ == 0)
		return;

	if (width_ == s_displayWidth && height_ == s_displayHeight)
		return;

	m_commandManager.IdleGPU();

	s_displayWidth = width_;
	s_displayHeight = height_;

	m_preDisplayBuffer.Create(L"Pre display buffer", width_, height_, 1, swapChainFormat);

	for (uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		m_displayPlane[i].ResetResource();
	}

	if (FAILED(sm_swapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, width_, height_, swapChainFormat, 0)))
		Debug::Error("Could not resize buffers!", __FILENAME__, __LINE__);

	for (uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> tempDisplayPlane;

		if (FAILED(sm_swapChain->GetBuffer(i, IID_PPV_ARGS(&tempDisplayPlane))))
			Debug::Error("Could not retrieve swap chain buffer " + i, __FILENAME__, __LINE__);
		
		m_displayPlane[i].CreateFromSwapChain(L"Primary swap chain buffer", tempDisplayPlane.Detach());
		m_displayPlane[i].SetClearColour(Colour(1.0f, 0.33f, 0.67f));
	}

	m_sceneDepthBuffer.Create(L"Scene depth buffer", s_displayWidth, s_displayHeight, DXGI_FORMAT_D32_FLOAT);
	m_presentBuffer.Create(L"Present buffer", s_displayWidth, s_displayHeight, 1, DXGI_FORMAT_R11G11B10_FLOAT);
	m_overlayBuffer.Create(L"Overlay buffer", s_displayWidth, s_displayHeight, 1, DXGI_FORMAT_R8G8B8A8_UNORM);

	s_currentBuffer = 0;
	m_commandManager.IdleGPU();
}

void DXGraphics::PreparePresent()
{
	GraphicsContext& context = GraphicsContext::RequestContext(L"Present");

	context.TransitionResource(m_presentBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	context.TransitionResource(m_overlayBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	context.TransitionResource(m_displayPlane[s_currentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	
	context.SetRootSignature(m_presentRootSig);
	context.SetDynamicDescriptor(0, 0, m_presentBuffer.GetSRV());
	context.SetDynamicDescriptor(0, 1, m_overlayBuffer.GetSRV());

	context.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context.SetPipelineState(m_presentPSO);
	context.SetRenderTargets(1, &m_displayPlane[s_currentBuffer].GetRTV());
	context.SetViewportAndScissor(0, 0, s_displayWidth, s_displayHeight);

	context.Draw(3);

	context.TransitionResource(m_displayPlane[s_currentBuffer], D3D12_RESOURCE_STATE_PRESENT);

	context.Finish(true);
}

void DXGraphics::Present()
{
	PreparePresent();

	s_currentBuffer = (s_currentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
	double frameTime = Graphics::GetFrameTime();
	UINT presentInterval = s_enableVSync ? std::min(4, (int)round(frameTime * REFRESH_RATE)) : 0;

	sm_swapChain->Present(presentInterval, 0);

	if (s_enableVSync) frameTime = 1000.0f * (1.0 / REFRESH_RATE);
	else frameTime = 1000.0f * OdeumEngine::Get().GetTimer().GetDeltaTime();

	if (frameCounter == 0)
	{
		frameTimeAverage = frameTimeTotal / (float)NUM_FRAMES_FOR_AVERAGES;
		frameTimeTotal = 0.0f;
	}

	frameTimeTotal += frameTime;
	frameCounter = (frameCounter + 1) % NUM_FRAMES_FOR_AVERAGES;

	Graphics::SetFrameTime((float)frameTime);
}

float DXGraphics::GetFrameRate()
{
	return (1.0f / frameTimeAverage) * 1000.0f;
}

float DXGraphics::GetFrameTime()
{
	return frameTimeAverage;
}

void DXGraphics::InitializeCommonState()
{
	alphaBlend.IndependentBlendEnable = FALSE;
	alphaBlend.RenderTarget[0].BlendEnable = FALSE;
	alphaBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	alphaBlend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	alphaBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	alphaBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	alphaBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	alphaBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	alphaBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendPreMultiplied = alphaBlend;

	depthReadWrite.DepthEnable = FALSE;
	depthReadWrite.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthReadWrite.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthReadWrite.StencilEnable = FALSE;
	depthReadWrite.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthReadWrite.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthReadWrite.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthReadWrite.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthReadWrite.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthReadWrite.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthReadWrite.BackFace = depthReadWrite.FrontFace;

	depthReadOnly = depthReadWrite;
	depthReadOnly.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	rasterDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rasterDesc.CullMode = D3D12_CULL_MODE_NONE;

	rasterTwoSided = rasterDesc;
	rasterTwoSided.CullMode = D3D12_CULL_MODE_NONE;

	DispatchIndirectCommandSignature[0].SetType(D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH);
	DispatchIndirectCommandSignature.Finalize();

	DrawIndirectCommandSignature[0].SetType(D3D12_INDIRECT_ARGUMENT_TYPE_DRAW);
	DrawIndirectCommandSignature.Finalize();
}

void DXGraphics::Shutdown()
{
	m_commandManager.IdleGPU();
	sm_swapChain->SetFullscreenState(false, nullptr);

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CommandContext::DestroyAllContexts();
	m_commandManager.Uninitialize();
	sm_swapChain->Release();

	DescriptorAllocator::DestroyHeaps();

	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_displayPlane[i].ResetResource();

	m_preDisplayBuffer.ResetResource();
	m_presentBuffer.ResetResource();
	m_sceneDepthBuffer.ResetResource();

	if (m_device != nullptr) m_device->Release(); m_device = nullptr;
}