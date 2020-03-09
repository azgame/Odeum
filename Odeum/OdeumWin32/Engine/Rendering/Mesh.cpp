#include "Mesh.h"
#include "../Utilities/d3dx12.h"

using namespace DirectX;

Mesh::Mesh(SubMesh subMesh_)
{
	subMesh = subMesh_;
}

Mesh::Mesh(const Mesh &)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	if (!InitializeBuffers(device, commandList)) return false;
	return true;
}

void Mesh::Shutdown() { ShutdownBuffers(); }

void Mesh::Render(ID3D12GraphicsCommandList* m_commandList) { RenderBuffers(m_commandList); }

bool Mesh::InitializeBuffers(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	HRESULT result;

	Vertex vertices[] = {
		// top
		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{  1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },

		// bottom
		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
		{  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },
		{  1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },

		// left
		{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },

		// right
		{  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{  1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },

		// front
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
		{  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
		{  1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },

		// back
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
		{  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
		{  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
	};

	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(subMesh.vertexList.size() * sizeof(Vertex));
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

	CD3DX12_RESOURCE_DESC m_indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(subMesh.meshIndices.size() * sizeof(UINT16));

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
	memcpy(pVertexDataBegin, subMesh.vertexList.data(), subMesh.vertexList.size() * sizeof(Vertex));
	m_vertexBufferUpload->Unmap(0, nullptr);
	commandList->CopyBufferRegion(m_vertexBuffer, 0, m_vertexBufferUpload, 0, subMesh.vertexList.size() * sizeof(Vertex));
	
	m_vertexBufferView.BufferLocation = m_vertexBufferUpload->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_vertexBufferView.SizeInBytes = subMesh.vertexList.size() * sizeof(Vertex);

	UINT8* pIndexDataBegin;
	result = m_indexBufferUpload->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
	if (FAILED(result)) return false;
	memcpy(pIndexDataBegin, subMesh.meshIndices.data(), subMesh.meshIndices.size() * sizeof(UINT16));
	m_indexBufferUpload->Unmap(0, nullptr);
	commandList->CopyBufferRegion(m_indexBuffer, 0, m_indexBufferUpload, 0, subMesh.meshIndices.size() * sizeof(UINT16));

	m_indexBufferView.BufferLocation = m_indexBufferUpload->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = subMesh.meshIndices.size() * sizeof(UINT16);
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_vertexCount = m_vertexBuffer->GetDesc().Width / sizeof(Vertex);
	m_indexCount = m_indexBuffer->GetDesc().Width / sizeof(UINT16);

	m_texture = TextureHandler::GetInstance()->LoadTexture("Engine/Resources/Textures/Apple_Body.jpg");

	D3D12_RESOURCE_DESC textureDesc;
	textureDesc = {};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = m_texture.width;
	textureDesc.Height = m_texture.height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Alignment = 0;

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_textureBuffer));
	if (FAILED(result)) return false;

	textureDesc.Width = m_texture.height * m_texture.width * m_texture.stride;
	textureDesc.Height = 1;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	textureDesc.Format = DXGI_FORMAT_UNKNOWN;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_textureBufferUpload));
	if (FAILED(result)) return false;

	UINT8* pTextureDataBegin;
	result = m_textureBufferUpload->Map(0, &readRange, reinterpret_cast<void**>(&pTextureDataBegin));
	if (FAILED(result)) return false;
	memcpy(pTextureDataBegin, m_texture.pixels.data(), m_texture.height * m_texture.width * m_texture.stride);
	m_textureBufferUpload->Unmap(0, nullptr);
	
	// Describe the upload heap resource location for the copy
	D3D12_SUBRESOURCE_FOOTPRINT subresource = {};
	subresource.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	subresource.Width = m_texture.width;
	subresource.Height = m_texture.height;
	subresource.RowPitch = m_texture.width * m_texture.stride;
	subresource.Depth = 1;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
	footprint.Offset = m_texture.offset;
	footprint.Footprint = subresource;

	D3D12_TEXTURE_COPY_LOCATION source = {};
	source.pResource = m_textureBufferUpload;
	source.PlacedFootprint = footprint;
	source.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;

	// Describe the default heap resource location for the copy
	D3D12_TEXTURE_COPY_LOCATION destination = {};
	destination.pResource = m_textureBuffer;
	destination.SubresourceIndex = 0;
	destination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

	// Copy the buffer resource from the upload heap to the texture resource on the default heap
	commandList->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);

	// Transition the texture to a shader resource
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_textureBuffer;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &barrier);

	m_textureView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_textureView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_textureView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_textureView.Texture2D.MipLevels = 1;
	m_textureView.Texture2D.MostDetailedMip = 0;
	m_textureView.Texture2D.PlaneSlice = 0;

	return true;
}

void Mesh::ShutdownBuffers()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_vertexBufferUpload);
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_indexBufferUpload);
	SAFE_RELEASE(m_textureBuffer);
	SAFE_RELEASE(m_textureBufferUpload);
}

void Mesh::RenderBuffers(ID3D12GraphicsCommandList* m_commandList)
{
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_commandList->IASetIndexBuffer(&m_indexBufferView);
	m_commandList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}
