#include "TestRender.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Rendering/DirectX12/Buffers/ColourBuffer.h"
#include "../Rendering/DirectX12/Buffers/DepthBuffer.h"

TestRender::TestRender()
{
	m_model = Model();
}

TestRender::~TestRender()
{
	m_rootSig.Destroy();
	m_colourPSO.Destroy();
}

void TestRender::Initialize()
{
	m_rootSig.Reset(0, 0);
	m_rootSig.Finalize(L"Colour viewer", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	DXGI_FORMAT colourFormat = DXGraphics::m_presentBuffer.GetFormat();
	DXGI_FORMAT depthFormat = DXGraphics::m_sceneDepthBuffer.GetFormat();

	Vertex mesh[3] = {
		Vertex(0.0f, 0.75f, 0.0f),
		Vertex(0.5f, -0.25f, 0.0f),
		Vertex(-0.5f, -0.25f, 0.0f)
	};

	uint16_t indices[3] = {
		0, 1, 2
	};

	m_model.Load(mesh, sizeof(mesh), sizeof(Vertex), indices, sizeof(indices));

	D3D12_INPUT_ELEMENT_DESC verts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	m_colourPSO.SetRootSignature(m_rootSig);
	m_colourPSO.SetRasterizerState(DXGraphics::rasterDesc);
	m_colourPSO.SetBlendState(DXGraphics::alphaBlend);
	m_colourPSO.SetDepthStencilState(DXGraphics::depthStateDisabled);
	m_colourPSO.SetInputLayout(_countof(verts), verts);
	m_colourPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_colourPSO.SetRenderTargetFormats(1, &colourFormat, DXGI_FORMAT_UNKNOWN);
	m_colourPSO.CompileVertexShader(L"Engine/Shaders/VertexShader.hlsl", "main", "vs_5_0");
	m_colourPSO.CompilePixelShader(L"Engine/Shaders/PixelShader.hlsl", "main", "ps_5_0");
	m_colourPSO.Finalize();

	m_mainViewport.Width = (float)DXGraphics::m_presentBuffer.GetWidth();
	m_mainViewport.Height = (float)DXGraphics::m_presentBuffer.GetHeight();
	m_mainViewport.MinDepth = 0.0f;
	m_mainViewport.MaxDepth = 0.0f;

	m_mainScissor.left = 0;
	m_mainScissor.top = 0;
	m_mainScissor.right = (LONG)DXGraphics::m_presentBuffer.GetWidth();
	m_mainScissor.bottom = (LONG)DXGraphics::m_presentBuffer.GetHeight();
}

void TestRender::Render()
{
	GraphicsContext& graphics = GraphicsContext::RequestContext(L"Scene Render");

	graphics.TransitionResource(DXGraphics::m_presentBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	graphics.ClearColor(DXGraphics::m_presentBuffer);
	
	graphics.SetRootSignature(m_rootSig);
	graphics.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics.SetIndexBuffer(m_model.m_indexBuffer.IndexBufferView());
	graphics.SetVertexBuffer(0, m_model.m_vertexBuffer.VertexBufferView());

	graphics.SetPipelineState(m_colourPSO);
	graphics.SetRenderTarget(DXGraphics::m_presentBuffer.GetRTV());
	graphics.SetViewportAndScissor(m_mainViewport, m_mainScissor);
	
	Model::Mesh& mesh = *m_model.m_mesh;
	uint32_t vertexStride = m_model.m_vertexStride;

	uint32_t indexCount = mesh.indexCount;
	uint32_t startIndex = mesh.indexDataByteOffset / sizeof(uint16_t);
	uint32_t baseVertex = mesh.vertexDataByteOffset / vertexStride;

	graphics.DrawIndexed(indexCount, startIndex, baseVertex);

	graphics.Finish();
}
