#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
#include "DeviceResources.h"
#include "Model.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(int, int, HWND);
	void Uninitialize();
	bool Frame();

private:
	bool Render();

	ID3D12Device*								m_device;
	DeviceResources*							m_deviceResources;
	ID3D12GraphicsCommandList*					m_commandList;
	ID3D12RootSignature*						m_rootSignature;
	ID3D12PipelineState*						m_pipelineState;
	Model*										m_triangle;
	D3D12_RECT									m_scissorRect;
	unsigned int								m_bufferIndex;
};

#endif // !_RENDERER_H_