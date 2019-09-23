#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include "pch.h"
//#include <wrl.h>
//#include <wrl/client.h>


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

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Shutdown();
	void Render(ID3D12GraphicsCommandList*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D12Device*, ID3D12GraphicsCommandList*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D12GraphicsCommandList*);

private:
	/*Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferUpload;*/
	
	ID3D12Resource* m_vertexBuffer;
	ID3D12Resource* m_vertexBufferUpload;
	
	ID3D12Resource* m_indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	int m_vertexCount, m_indexCount;
};

#endif