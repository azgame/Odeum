#include "TestRender.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Rendering/DirectX12/Buffers/ColourBuffer.h"
#include "../Rendering/DirectX12/Buffers/DepthBuffer.h"

TestRender::TestRender()
{
	m_model = Model();
	m_camera = Camera();

	m_eventFrameLimit = 16;
	m_eventQueue.reserve(m_eventFrameLimit);
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

	using namespace DirectX;

	Vertex mesh[] = {
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};

	uint16_t indices[] = {
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	m_model.Load(mesh, sizeof(mesh), sizeof(Vertex), indices, sizeof(indices));

	D3D12_INPUT_ELEMENT_DESC verts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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

	m_mainViewport.Width = (float)DXGraphics::m_presentBuffer.GetWidth();
	m_mainViewport.Height = (float)DXGraphics::m_presentBuffer.GetHeight();
	m_mainViewport.MinDepth = 0.0f;
	m_mainViewport.MaxDepth = 1.0f;

	m_mainScissor.left = 0;
	m_mainScissor.top = 0;
	m_mainScissor.right = (LONG)DXGraphics::m_presentBuffer.GetWidth();
	m_mainScissor.bottom = (LONG)DXGraphics::m_presentBuffer.GetHeight();

	Vector3 eye = Vector3(2.0f, 1.0f, 5.0f);
	m_camera.SetEyeAtUp(eye, Vector3(kZero), Vector3(kYUnitVector));
	m_camera.SetZRange(0.1f, 10000.0f);
}

void TestRender::Detach()
{
	m_rootSig.Destroy();
	m_colourPSO.Destroy();
}

void TestRender::Update(float deltaTime_)
{
	int limit = 0;
	for (int i = m_bufferHead; i != m_bufferTail; i = (i + 1) % m_eventFrameLimit)
	{
		if (limit == m_eventFrameLimit)
			break;

		// Handle events

		limit++;
	}

	m_camera.UpdateCamera();

	struct VSConstants
	{
		DirectX::XMMATRIX modelToProjection;
	} vsConstants;
	vsConstants.modelToProjection = m_camera.GetViewProjMatrix();

	GraphicsContext& graphics = GraphicsContext::RequestContext(L"Scene Render");

	graphics.TransitionResource(DXGraphics::m_presentBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	graphics.TransitionResource(DXGraphics::m_sceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	graphics.ClearColor(DXGraphics::m_presentBuffer);
	graphics.ClearDepthAndStencil(DXGraphics::m_sceneDepthBuffer);

	graphics.SetRootSignature(m_rootSig);

	graphics.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);

	graphics.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics.SetIndexBuffer(m_model.m_indexBuffer.IndexBufferView());
	graphics.SetVertexBuffer(0, m_model.m_vertexBuffer.VertexBufferView());

	graphics.SetPipelineState(m_colourPSO);

	graphics.SetRenderTarget(DXGraphics::m_presentBuffer.GetRTV(), DXGraphics::m_sceneDepthBuffer.GetDSV());
	graphics.SetViewportAndScissor(m_mainViewport, m_mainScissor);

	Model::Mesh& mesh = *m_model.m_mesh;
	uint32_t vertexStride = m_model.m_vertexStride;

	uint32_t indexCount = mesh.indexCount;
	uint32_t startIndex = mesh.indexDataByteOffset / sizeof(uint16_t);
	uint32_t baseVertex = mesh.vertexDataByteOffset / vertexStride;

	graphics.DrawIndexed(indexCount, startIndex, baseVertex);

	graphics.Finish();
}

void TestRender::UIRender()
{
}

void TestRender::HandleEvent(Event& event_)
{
	if ((m_bufferTail + 1) % m_eventFrameLimit == m_bufferHead)
		return;

	m_eventQueue[m_bufferTail] = event_;

	m_bufferTail = (m_bufferTail + 1) % m_eventFrameLimit;
}