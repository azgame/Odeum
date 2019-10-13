#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
#include "DeviceResources.h"
#include "Model.h"
#include "Camera.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

struct ModelViewProjectionConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(int, int, HWND);
	void CreateWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	void Uninitialize();
	bool Frame();

private:
	void Update();
	bool Render();

	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

	ID3D12Device*								m_device;
	DeviceResources*							m_deviceResources;
	ID3D12GraphicsCommandList*					m_commandList;
	ID3D12RootSignature*						m_rootSignature;
	ID3D12PipelineState*						m_pipelineState;
	ID3D12DescriptorHeap*						m_cbvHeap;
	ID3D12Resource*								m_constantBuffer;
	ModelViewProjectionConstantBuffer			m_constantBufferData;
	UINT8*										m_mappedConstantBuffer;
	UINT										m_cbvDescriptorSize;
	Model*										m_triangle;
	D3D12_RECT									m_scissorRect;
	unsigned int								m_bufferIndex;
	Camera*										m_camera;
};

#endif // !_RENDERER_H_