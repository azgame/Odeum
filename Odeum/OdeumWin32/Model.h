#ifndef _MODEL_H_
#define _MODEL_H_

#include "pch.h"


struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

class Model 
{
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Shutdown();
	void Render(ID3D12GraphicsCommandList*);

	int GetIndexCount()						{ return m_indexCount; }
	int GetVertexCount()					{ return m_vertexCount; }
	ID3D12Resource* GetVertexBuffer()		{ return m_vertexBuffer; }
	ID3D12Resource* GetIndexBuffer()		{ return m_indexBuffer; }
	D3D12_VERTEX_BUFFER_VIEW GetVertexBV()	{ return m_vertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW GetIndexBV()	{ return m_indexBufferView; }

private:
	bool InitializeBuffers(ID3D12Device*, ID3D12GraphicsCommandList*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D12GraphicsCommandList*);
	
	ID3D12Resource*							m_vertexBuffer;
	ID3D12Resource*							m_vertexBufferUpload;
	D3D12_VERTEX_BUFFER_VIEW				m_vertexBufferView;
	ID3D12Resource*							m_indexBuffer;
	ID3D12Resource*							m_indexBufferUpload;
	D3D12_INDEX_BUFFER_VIEW					m_indexBufferView;

	int										m_vertexCount, m_indexCount;
};

#endif