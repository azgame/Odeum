#include "TestRender.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Rendering/DirectX12/Buffers/ColourBuffer.h"
#include "../Rendering/DirectX12/Buffers/DepthBuffer.h"

#include "../Rendering/DirectX12/SceneGraph.h"

#include "OdeumEngine.h"

TestRender::TestRender()
{
	m_eventFrameLimit = 16;
	m_eventQueue.resize(m_eventFrameLimit);
	m_bufferHead = 0;
	m_bufferTail = 0;
}

TestRender::~TestRender()
{
}

void TestRender::Attach()
{
	m_rootSig.Reset(1, 0);
	m_rootSig[0].InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	m_rootSig.Finalize(L"Colour viewer", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	DXGI_FORMAT colourFormat = DXGraphics::m_presentBuffer.GetFormat();
	DXGI_FORMAT depthFormat = DXGraphics::m_sceneDepthBuffer.GetFormat();

	D3D12_INPUT_ELEMENT_DESC verts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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

	Vector3 eye = Vector3(0.0f, 3.0f, 10.0f);
	OdeumEngine::Get().GetCamera().SetEyeAtUp(eye, Vector3(kZero), Vector3(kYUnitVector));
	OdeumEngine::Get().GetCamera().SetZRange(0.1f, 10000.0f);
}

void TestRender::Detach()
{
	m_rootSig.Destroy();
	m_colourPSO.Destroy();
}

void TestRender::Update(float deltaTime_)
{
	while (m_bufferHead != m_bufferTail)
	{
		// Handle events here
		m_bufferHead = (m_bufferHead + 1) % m_eventFrameLimit;
	}

	m_mainViewport.Width = (float)DXGraphics::m_presentBuffer.GetWidth();
	m_mainViewport.Height = (float)DXGraphics::m_presentBuffer.GetHeight();
	m_mainViewport.MinDepth = 0.0f;
	m_mainViewport.MaxDepth = -1.0f;

	m_mainScissor.left = 0;
	m_mainScissor.top = 0;
	m_mainScissor.right = (LONG)DXGraphics::m_presentBuffer.GetWidth();
	m_mainScissor.bottom = (LONG)DXGraphics::m_presentBuffer.GetHeight();

	struct VSConstants
	{
		DirectX::XMMATRIX viewProj;
		DirectX::XMMATRIX model;
	} vsConstants;
	vsConstants.viewProj = OdeumEngine::Get().GetCamera().GetViewProjMatrix();

	GraphicsContext& graphics = GraphicsContext::RequestContext(L"Scene Render");

	graphics.TransitionResource(DXGraphics::m_presentBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	graphics.TransitionResource(DXGraphics::m_sceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	graphics.ClearColor(DXGraphics::m_presentBuffer);
	graphics.ClearDepth(DXGraphics::m_sceneDepthBuffer);

	graphics.SetRootSignature(m_rootSig);
	graphics.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics.SetPipelineState(m_colourPSO);

	graphics.SetRenderTarget(DXGraphics::m_presentBuffer.GetRTV(), DXGraphics::m_sceneDepthBuffer.GetDSV());
	graphics.SetViewportAndScissor(m_mainViewport, m_mainScissor);

	for (auto object : SceneGraph::Get()->GetGameObjects())
	{
		vsConstants.model = object->GetTransform();
		graphics.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);

		graphics.SetIndexBuffer(object->GetModel().m_indexBuffer.IndexBufferView());
		graphics.SetVertexBuffer(0, object->GetModel().m_vertexBuffer.VertexBufferView());

		for (int i = 0; i < object->GetModel().m_pDetails[0].meshCount; i++)
		{
			Model::Mesh& mesh = object->GetModel().GetMesh(i);
			uint32_t vertexStride = object->GetModel().m_vertexStride;
			uint32_t indexCount = mesh.indexCount;
			uint32_t startIndex = mesh.indexDataByteOffset / sizeof(uint32_t);
			uint32_t baseVertex = mesh.vertexDataByteOffset / sizeof(Vertex);

			graphics.DrawIndexed(indexCount, startIndex, baseVertex);
		}	
	}

	graphics.Finish();
}

void TestRender::UIRender()
{
}

void TestRender::HandleEvent(Event& event_) // Queue events
{
	if ((m_bufferTail + 1) % m_eventFrameLimit == m_bufferHead)
		return;

	m_eventQueue[m_bufferTail] = &event_;

	m_bufferTail = (m_bufferTail + 1) % m_eventFrameLimit;
}