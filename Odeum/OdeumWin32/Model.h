#ifndef _MODEL_H_
#define _MODEL_H_

#include "pch.h"



class Model 
{
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Shutdown();
	void Render(ID3D12GraphicsCommandList*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D12Device*, ID3D12GraphicsCommandList*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D12GraphicsCommandList*);
	
	ID3D12Resource* m_vertexBuffer;
	ID3D12Resource* m_vertexBufferUpload;
	
	ID3D12Resource* m_indexBuffer;
	ID3D12Resource* m_indexBufferUpload;

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	int m_vertexCount, m_indexCount;
};

#endif