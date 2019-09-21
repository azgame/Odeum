#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include "pch.h"


class Model 
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D12Device*);
	void Shutdown();
	void Render(ID3D12GraphicsCommandList*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D12Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D12GraphicsCommandList*);

private:
	ID3D12Resource *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif