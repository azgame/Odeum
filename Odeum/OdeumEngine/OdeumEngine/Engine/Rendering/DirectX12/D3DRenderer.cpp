#include "D3DRenderer.h"

#include "D3DCore.h"
#include "Buffers/ColourBuffer.h"
#include "Buffers/DepthBuffer.h"
#include "LightSource.h"
#include "SceneGraph.h"
#include "SamplerDesc.h"
#include "ParticleManager.h"
#include "RootSignature.h"

#include "../../Core/Camera.h"
#include "../../Core/Window.h"

void D3DRenderer::Initialize(Window& Window)
{
	SamplerDesc defaultSampler;
	defaultSampler.MaxAnisotropy = 8;

	m_rootSig.Reset(3, 1);
	m_rootSig.InitStaticSampler(0, defaultSampler, D3D12_SHADER_VISIBILITY_PIXEL);
	m_rootSig[0].InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	m_rootSig[1].InitAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_rootSig[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 4, D3D12_SHADER_VISIBILITY_PIXEL);
	m_rootSig.Finalize(L"Colour viewer", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	DXGI_FORMAT colourFormat = DXGraphics::m_presentBuffer.GetFormat();
	DXGI_FORMAT depthFormat = DXGraphics::m_sceneDepthBuffer.GetFormat();

	D3D12_INPUT_ELEMENT_DESC verts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	m_colourPSO.SetRootSignature(m_rootSig);
	m_colourPSO.SetRasterizerState(DXGraphics::rasterDesc);
	m_colourPSO.SetBlendState(DXGraphics::alphaBlend);
	m_colourPSO.SetDepthStencilState(DXGraphics::depthReadWrite);
	m_colourPSO.SetInputLayout(_countof(verts), verts);
	m_colourPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_colourPSO.SetRenderTargetFormats(1, &colourFormat, depthFormat);
	m_colourPSO.CompileVertexShader(L"Engine/Shaders/VertexShader.hlsl", "main", "vs_5_0");
	m_colourPSO.CompilePixelShader(L"Engine/Shaders/PixelShader.hlsl", "main", "ps_5_0");
	m_colourPSO.Finalize();

	CreateUIResources();
	InitializeUI(Window);
}

void D3DRenderer::Update(float deltaTime)
{
}

void D3DRenderer::Render(Camera& Camera, float deltaTime)
{
	SCOPEDTIMER(timer);

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_mainViewport.Width = (float)DXGraphics::m_presentBuffer.GetWidth();
	m_mainViewport.Height = (float)DXGraphics::m_presentBuffer.GetHeight();
	m_mainViewport.MinDepth = -1.0f;
	m_mainViewport.MaxDepth = 0.0f;

	m_mainScissor.left = 0;
	m_mainScissor.top = 0;
	m_mainScissor.right = (LONG)DXGraphics::m_presentBuffer.GetWidth();
	m_mainScissor.bottom = (LONG)DXGraphics::m_presentBuffer.GetHeight();

	struct VSConstants
	{
		DirectX::XMMATRIX viewProj;
		DirectX::XMMATRIX model;
		DirectX::XMFLOAT3 viewerPos;
	} vsConstants;

	vsConstants.viewProj = Camera.GetViewProjMatrix();
	DirectX::XMStoreFloat3(&vsConstants.viewerPos, Camera.GetPosition());

	LightData light;
	DirectX::XMStoreFloat3(&light.position, Vector3(10.0f, 20.0f, -30.0f));
	light.radiusSq = 4000.0f;
	DirectX::XMStoreFloat3(&light.colour, Vector3(0.3f, 0.3f, 0.3f));

	GraphicsContext& graphics = GraphicsContext::RequestContext(L"Scene Render");

	ParticleManager::Get().Update(graphics.GetComputeContext(), deltaTime);

	graphics.TransitionResource(DXGraphics::m_presentBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	graphics.TransitionResource(DXGraphics::m_sceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	graphics.ClearColor(DXGraphics::m_presentBuffer);
	graphics.ClearDepth(DXGraphics::m_sceneDepthBuffer);

	graphics.SetRootSignature(m_rootSig);
	graphics.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics.SetPipelineState(m_colourPSO);

	graphics.SetRenderTarget(DXGraphics::m_presentBuffer.GetRTV(), DXGraphics::m_sceneDepthBuffer.GetDSV());
	graphics.SetViewportAndScissor(m_mainViewport, m_mainScissor);

	graphics.SetDynamicConstantBufferView(1, sizeof(light), &light);

	for (auto object : SceneGraph::Get()->GetRenderObjects())
	{
		if (object->IsRendered())
		{
			vsConstants.model = object->GetGameObject()->GetTransform();
			graphics.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);

			graphics.SetIndexBuffer(object->GetModel().m_indexBuffer.IndexBufferView());
			graphics.SetVertexBuffer(0, object->GetModel().m_vertexBuffer.VertexBufferView());

			for (UINT i = 0; i < object->GetModel().m_details.meshCount; i++)
			{
				Model::Mesh& mesh = object->GetModel().GetMesh(i);
				uint32_t vertexStride = object->GetModel().m_vertexStride;
				uint32_t indexCount = mesh.indexCount;
				uint32_t startIndex = mesh.indexOffset;
				uint32_t baseVertex = mesh.vertexOffset;

				graphics.SetDynamicDescriptors(2, 0, 4, object->GetModel().GetSRVs(mesh.materialIndex));

				graphics.DrawIndexed(indexCount, startIndex, baseVertex);
			}
		}	
	}

	ParticleManager::Get().Render(graphics, Camera, DXGraphics::m_presentBuffer, DXGraphics::m_sceneDepthBuffer);

	graphics.Finish();

	if (frameCounter == 0)
	{
		averageFrameTime = frameTimeTotal / 120.0f;
		frameTimeTotal = 0.0f;
		averageNumParticles = totalNumParticles / 120.f;
		totalNumParticles = 0.0f;
	}

	frameTimeTotal += timer.GetTime();
	totalNumParticles += ParticleManager::Get().GetTotalNumParticles();
	frameCounter = (frameCounter + 1) % 120;
}

void D3DRenderer::UIRender()
{
	ImGui::Begin("Frame Profiling");

	ImGui::Text("Frame time: %.2f ms/frame", DXGraphics::GetFrameTime());
	ImGui::Text("FPS: %.1f fps", DXGraphics::GetFrameRate());
	ImGui::Text("Render Pass frame time: %.2f ms/frame", averageFrameTime);
	ImGui::Text("Total Number of Particles: %.0f", averageNumParticles);

	ImGui::End();

	UIRenderD3DResources();
}

void D3DRenderer::Uninitialize()
{
}

void D3DRenderer::CreateUIResources()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	if (FAILED(DXGraphics::m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pHeap))))
	{
		ERROR("Could not create desc heap for UI render!");
		throw std::runtime_error("Could not create desc heap for UI render!");
	}
}

void D3DRenderer::InitializeUI(Window& Window)
{
	// Imgui integration
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(Window.GetHWND());
	ImGui_ImplDX12_Init(DXGraphics::m_device, SWAP_CHAIN_BUFFER_COUNT, DXGraphics::m_presentBuffer.GetFormat(),
		m_pHeap, m_pHeap->GetCPUDescriptorHandleForHeapStart(), m_pHeap->GetGPUDescriptorHandleForHeapStart());
}

void D3DRenderer::UIRenderD3DResources()
{
	GraphicsContext& uiContext = CommandContext::RequestContext(L"UI context").GetGraphicsContext();

	//uiContext.TransitionResource(DXGraphics::m_overlayBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	//uiContext.ClearColor(DXGraphics::m_overlayBuffer);
	uiContext.SetRenderTarget(DXGraphics::m_presentBuffer.GetRTV());
	ID3D12GraphicsCommandList* cmdList = uiContext.GetCommandList();
	cmdList->SetDescriptorHeaps(1, &m_pHeap);
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);

	uiContext.Finish();
}
