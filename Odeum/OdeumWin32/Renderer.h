#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
#include "DeviceResources.h"
#include "Model.h"
#include "Camera.h"
#include "RaytracingHLSLCompat.h"
#include "DXStructures.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const bool dxrEnabled = true;

namespace GlobalRootSignatureParams {
	enum Value {
		OutputViewSlot = 0,
		AccelerationStructureSlot,
		SceneConstantSlot,
		VertexBuffersSlot,
		Count
	};
}

namespace LocalRootSignatureParams {
	enum Value {
		CubeConstantSlot = 0,
		Count
	};
}

namespace RootSignatureParams {
	enum Value {
		OutputViewSlot = 0,
		AccelerationStructureSlot,
		SceneConstantSlot,
		VertexBuffersSlot,
		CubeConstantSlot,
		Count
	};
}

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(int, int, HWND, std::vector<Model*>);
	void CreateRasterWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	bool CreateRaytracingWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	void Uninitialize();
	bool UpdateConstantResources();
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
	int											m_screenHeight, m_screenWidth;

	// General functions
	bool InitializeDeviceResources(int, int, HWND, bool, bool);

	// Constant Buffer for Camera
	static const UINT							c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;
	ID3D12DescriptorHeap*						m_descHeap;
	UINT										m_descHeapSize;
	UINT										m_descHeapAllocated;
	ID3D12DescriptorHeap*						m_cbvHeap;
	UINT										m_cbvHeapSize;
	UINT										m_cbvHeapAllocated;
	ID3D12Resource*								m_constantBuffer;
	ModelViewProjectionConstantBuffer			m_constantBufferData;
	UINT8*										m_mappedConstantBuffer;

	// Rasterization related functions and variables
	ID3D12GraphicsCommandList4*					m_commandList;
	ID3D12RootSignature*						m_rootSignature;
	ID3D12PipelineState*						m_pipelineState;

	// DXR related variables
	// Attributes
	// Root Signatures
	ID3D12RootSignature*						m_raytracingGlobalRootSignature;
	ID3D12RootSignature*						m_raytracingLocalRootSignature;

	union AlignedSceneConstantBuffer
	{
		SceneConstantBuffer constants;
		uint8_t alignmentPadding[D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT];
	};

	// Raytracing scene
	RayGenConstantBuffer						m_rayGenCB; /// Nvidia
	SceneConstantBuffer							m_sceneCB[c_frameCount]; /// Microsoft
	AlignedSceneConstantBuffer*					m_dxrmappedConstantBuffer;
	CubeConstantBuffer							m_cubeCB; /// Microsoft

	// Raytracing constant resources
	ID3D12Resource*								m_dxrconstantBuffer;

	// Acceleration structures
	ID3D12Resource*								m_accelerationStructure;
	ID3D12Resource*								m_bottomLevelAccelerationStructure;
	ID3D12Resource*								m_topLevelAccelerationStructure;

	// Raytracing output
	ID3D12Resource*								m_raytracingOutput;
	D3D12_GPU_DESCRIPTOR_HANDLE					m_raytracingOutputResourceUAVGpuDescriptor;
	UINT										m_raytracingOutputResourceUAVDescriptorHeapIndex;

	// Shader tables
	static const wchar_t*						c_hitGroupName;
	static const wchar_t*						c_raygenShaderName;
	static const wchar_t*						c_closestHitShaderName;
	static const wchar_t*						c_missShaderName;
	ID3D12Resource*								m_missShaderTable;
	ID3D12Resource*								m_hitGroupShaderTable;
	ID3D12Resource*								m_rayGenShaderTable;
	ID3D12Resource*								m_shaderTable;
	uint32_t									m_shaderTableRecordSize = 0;

	// Changeable init
	bool InitializeRaster(int, int, HWND, std::vector<Model*> renderObjects);
	bool InitializeRaytrace(int, int, HWND, std::vector<Model*>);

	// Changeable Render
	bool RenderRaster(std::vector<Model*> renderObjects);
	bool RenderRaytrace(std::vector<Model*> renderObjects);

	// General
	bool CreateCBResources();
	
	// Raytracing related functions
	
	bool CreateRaytracingInterfaces(int screenHeight, int screenWidth, HWND hwnd);
	bool BuildAccelerationStructures(std::vector<Model*> renderObjects);
	bool SerializeAndCreateRaytracingRootSignature(D3D12_ROOT_SIGNATURE_DESC& desc, ID3D12RootSignature** rootSig);
	bool CreateRaytracingPipelineStateObject();
	bool CreateDescriptorHeap(std::vector<Model*> renderObjects);
	bool BuildShaderTables();
	bool DoRaytracing();
	bool CopyRaytracingOutputToBackbuffer();
	UINT AllocateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse = UINT_MAX);
	bool CreateRaytracingOutputResource();

	// Ray tracing pipeline state object and properties
	ID3D12StateObject*							m_rtStateObject;
	ID3D12StateObjectProperties*				m_rtStateObjectProps;
};

#endif // !_RENDERER_H_