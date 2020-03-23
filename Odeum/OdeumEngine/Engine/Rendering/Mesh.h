#ifndef _MESH_H_
#define _MESH_H_

#include "../../pch.h"

#include "../Utilities/DXRHelper.h"
#include "TextureHandler.h"

struct Vertex
{
	Vertex() {}
	Vertex(DirectX::XMFLOAT3 pos_, DirectX::XMFLOAT3 norm_, DirectX::XMFLOAT2 uv_) : position(pos_), normal(norm_), uv(uv_) {}
	Vertex(float x, float y, float z, float xn, float yn, float zn, float u, float v) : position(x, y, z), normal(xn, yn, zn), uv(u, v) {}

	bool operator==(Vertex v_)
	{
		bool posEquals = position.x == v_.position.x && position.y == v_.position.y && position.z == v_.position.z;
		bool normalEquals = normal.x == v_.normal.x && normal.y == v_.normal.y && normal.z == v_.normal.z;
		bool uvEquals = uv.x == v_.uv.x && uv.y == v_.uv.y;
		if (posEquals && normalEquals && uvEquals) return true;
		else return false;
	}

	bool operator!=(Vertex v_)
	{
		bool posEquals = position.x == v_.position.x && position.y == v_.position.y && position.z == v_.position.z;
		bool normalEquals = normal.x == v_.normal.x && normal.y == v_.normal.y && normal.z == v_.normal.z;
		bool uvEquals = uv.x == v_.uv.x && uv.y == v_.uv.y;
		if (posEquals && normalEquals && uvEquals) return false;
		else return true;
	}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

struct SubMesh
{
	std::vector<Vertex> vertexList;
	std::vector<UINT16> meshIndices;
	Texture m_texture;
};

class Mesh 
{
public:
	Mesh(SubMesh subMesh_);
	Mesh(const Mesh&);
	~Mesh();

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Shutdown();
	void Render(ID3D12GraphicsCommandList*);

	int GetIndexCount()						{ return m_indexCount; }
	int GetVertexCount()					{ return m_vertexCount; }
	ID3D12Resource* GetVertexBuffer()		{ return m_vertexBufferUpload; }
	ID3D12Resource* GetIndexBuffer()		{ return m_indexBufferUpload; }
	ID3D12Resource* GetTextureBuffer()		{ return m_textureBuffer; }
	D3D12_VERTEX_BUFFER_VIEW GetVertexBV()	{ return m_vertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW GetIndexBV()	{ return m_indexBufferView; }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetTextureBV()	{ return m_textureView; }

private:

	SubMesh subMesh;

	bool InitializeBuffers(ID3D12Device*, ID3D12GraphicsCommandList*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D12GraphicsCommandList*);
	
	// Todo Aidan: Move rendering out of mesh -> to mesh renderer
	ID3D12Resource*							m_vertexBuffer;
	ID3D12Resource*							m_vertexBufferUpload;
	D3D12_VERTEX_BUFFER_VIEW				m_vertexBufferView;
	ID3D12Resource*							m_indexBuffer;
	ID3D12Resource*							m_indexBufferUpload;
	D3D12_INDEX_BUFFER_VIEW					m_indexBufferView;
	int										m_vertexCount, m_indexCount;
	ID3D12Resource*							m_textureBuffer;
	ID3D12Resource*							m_textureBufferUpload;
	D3D12_SHADER_RESOURCE_VIEW_DESC			m_textureView;

};

#endif