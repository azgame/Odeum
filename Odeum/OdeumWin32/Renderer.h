#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
#include "DeviceResources.h"
#include "Model.h"
#include "Camera.h"
#include "RaytracingHLSLCompat.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


namespace GlobalRootSignatureParams {
	enum Value {
		OutputViewSlot = 0,
		AccelerationStructureSlot,
		Count
	};
}

namespace LocalRootSignatureParams {
	enum Value {
		ViewportConstantSlot = 0,
		Count
	};
}

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool InitializeRaster(int, int, HWND);
	bool InitializeRaytrace(int, int, HWND);
	void CreateWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	void Uninitialize();
	bool Render(std::vector<Model*> renderObjects);

	ID3D12Device*								GetD3DDevice() { return m_device; }
	ID3D12GraphicsCommandList*					GetCommandList() { return m_commandList; }

private:

	// General resources
	DeviceResources*							m_deviceResources;
	ID3D12Device5*								m_device;

	D3D12_RECT									m_scissorRect;
	unsigned int								m_bufferIndex;
	Camera*										m_camera;

	// General functions
	bool InitializeDeviceResources(int, int, HWND, bool, bool);

	// Constant Buffer for Camera
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;
	ID3D12DescriptorHeap*						m_cbvHeap;
	ID3D12Resource*								m_constantBuffer;
	ModelViewProjectionConstantBuffer			m_constantBufferData;
	UINT8*										m_mappedConstantBuffer;
	UINT										m_cbvDescriptorSize;

	// Rasterization related functions and variables
	ID3D12GraphicsCommandList*					m_commandList;
	ID3D12RootSignature*						m_rootSignature;
	ID3D12PipelineState*						m_pipelineState;

	// DXR related variables
	// Attributes
	ID3D12GraphicsCommandList4* m_dxrCommandList;
	ID3D12StateObject* m_dxrStateObject;

	// Root Signatures
	ID3D12RootSignature* m_raytracingGlobalRootSignature;
	ID3D12RootSignature* m_raytracingLocalRootSignature;

	// Raytracing scene
	RayGenConstantBuffer m_rayGenCB;

	// Acceleration structures
	ID3D12Resource* m_accelerationStructure;
	ID3D12Resource* m_bottomLevelAccelerationStructure;
	ID3D12Resource* m_topLevelAccelerationStructure;

	// Raytracing output
	ID3D12Resource* m_raytracingOutput;
	D3D12_GPU_DESCRIPTOR_HANDLE m_raytracingOutputResourceUAVGpuDescriptor;
	UINT m_raytracingOutputResourceUAVDescriptorHeapIndex;

	// Shader tables
	static const wchar_t* c_hitGroupName;
	static const wchar_t* c_raygenShaderName;
	static const wchar_t* c_closestHitShaderName;
	static const wchar_t* c_missShaderName;
	ID3D12Resource* m_missShaderTable;
	ID3D12Resource* m_hitGroupShaderTable;
	ID3D12Resource* m_rayGenShaderTable;
	
	// Raytracing related functions
	bool DoRaytracing();
	bool CreateRaytracingInterfaces(int screenHeight, int screenWidth, HWND hwnd);
	bool SerializeAndCreateRaytracingRootSignature(D3D12_ROOT_SIGNATURE_DESC& desc, ComPtr<ID3D12RootSignature>* rootSig);
	bool CreateRootSignatures();
	bool CreateLocalRootSignatureSubobjects(CD3DX12_STATE_OBJECT_DESC* raytracingPipeline);
	bool CreateRaytracingPipelineStateObject();
	bool CreateDescriptorHeap();
	bool CreateRaytracingOutputResource();
	bool BuildGeometry();
	bool BuildAccelerationStructures();
	bool BuildShaderTables();
	bool CopyRaytracingOutputToBackbuffer();
	UINT AllocateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse = UINT_MAX);
};

#endif // !_RENDERER_H_