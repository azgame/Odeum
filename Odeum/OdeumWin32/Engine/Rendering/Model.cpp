#include "Model.h"

using namespace DirectX;

Model::Model()
{
}

Model::Model(const Model &)
{
}


Model::~Model()
{
}

bool Model::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	if (!InitializeBuffers(device, commandList)) return false;
	
	return true;
}

void Model::Shutdown() { ShutdownBuffers(); }

void Model::Render(ID3D12GraphicsCommandList* m_commandList) { RenderBuffers(m_commandList); }

bool Model::InitializeBuffers(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	HRESULT result;

	VertexType vertices[] =
	{
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

	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));
	if (FAILED(result)) return false;

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBufferUpload));
	if (FAILED(result)) return false;

	UINT16 cubeIndices[] =
		{
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

	const UINT indexBufferSize = sizeof(cubeIndices);

	// Create the index buffer resource in the GPU's default heap and copy index data to it using the upload heap.
	// The upload resource must not be released until after the GPU has finished using it.

	CD3DX12_RESOURCE_DESC m_indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(cubeIndices));

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&m_indexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));
	if (FAILED(result)) return false;

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&m_indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBufferUpload));
	if (FAILED(result)) return false;

	UINT8* pVertexDataBegin;
	D3D12_RANGE readRange;
	readRange.Begin = 0;
	readRange.End = 0;
	result = m_vertexBufferUpload->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
	if (FAILED(result)) return false;
	memcpy(pVertexDataBegin, vertices, sizeof(vertices));
	m_vertexBufferUpload->Unmap(0, nullptr);
	commandList->CopyBufferRegion(m_vertexBuffer, 0, m_vertexBufferUpload, 0, sizeof(vertices));
	
	m_vertexBufferView.BufferLocation = m_vertexBufferUpload->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(VertexType);
	m_vertexBufferView.SizeInBytes = sizeof(vertices);

	UINT8* pIndexDataBegin;
	result = m_indexBufferUpload->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
	if (FAILED(result)) return false;
	memcpy(pIndexDataBegin, cubeIndices, sizeof(cubeIndices));
	m_indexBufferUpload->Unmap(0, nullptr);
	commandList->CopyBufferRegion(m_indexBuffer, 0, m_indexBufferUpload, 0, sizeof(cubeIndices));

	m_indexBufferView.BufferLocation = m_indexBufferUpload->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = sizeof(cubeIndices);
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_vertexCount = m_vertexBuffer->GetDesc().Width / sizeof(VertexType);
	m_indexCount = m_indexBuffer->GetDesc().Width / sizeof(UINT16);

	return true;
}

void Model::ShutdownBuffers()
{
}

void Model::RenderBuffers(ID3D12GraphicsCommandList* m_commandList)
{
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_commandList->IASetIndexBuffer(&m_indexBufferView);
	m_commandList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}
