#include "Model.h"



Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

Model::Model(const Model &)
{
}


Model::~Model()
{
}

bool Model::Initialize(ID3D12Device* device)
{
	if (!InitializeBuffers(device)) return false;
	
	return true;
}

void Model::Shutdown() { ShutdownBuffers(); }

void Model::Render(ID3D12GraphicsCommandList* m_commandList) { RenderBuffers(m_commandList); }

int Model::GetIndexCount() { return m_indexCount; }

bool Model::InitializeBuffers(ID3D12Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D12_RESOURCE_DESC vertexBufferDesc, indexBufferDesc;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	HRESULT result;

	m_vertexCount = 3;
	m_indexCount = 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f); // Bottom left
	vertices[0].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // Top middle
	vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f); // Bottom right
	vertices[2].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	
	indices[0] = 0; // Bottom left
	indices[1] = 1; // Top middle
	indices[2] = 2; // Bottom right

	
	

	return true;
}

void Model::ShutdownBuffers()
{
}

void Model::RenderBuffers(ID3D12GraphicsCommandList *)
{
}
