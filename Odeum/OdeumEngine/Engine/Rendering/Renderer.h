#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../../pch.h"
#include "../Utilities/DeviceResources.h"
#include "GameObject.h"
#include "../Core/Camera.h"
#include "../Utilities/RaytracingHLSLCompat.h"
#include "../Utilities/DXStructures.h"
#include "ShaderHandler.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const bool dxrEnabled = false;

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(int, int, HWND, std::vector<GameObject*>);
	void CreateWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	void Uninitialize();
	bool UpdateConstantResources();
	bool Render();

	ID3D12Device* GetD3DDevice()				{ return m_device; }
	DeviceResources* GetDeviceResources()		{ return m_deviceResources; }

private:
	// General resources
	DeviceResources*							m_deviceResources;
	ID3D12Device5*								m_device;

	unsigned int								m_bufferIndex;
	Camera*										m_camera;
	int											m_screenHeight, m_screenWidth;

	ID3D12DescriptorHeap*						m_descHeap;
	UINT										m_descHeapSize;
	UINT										m_descHeapAllocated;

	ID3D12GraphicsCommandList4*					m_commandList;

	std::vector<GameObject*>*					m_renderObjects;

	// General functions
	bool InitializeDeviceResources(int, int, HWND, bool, bool, bool);
	bool CreateCBResources();

	// Raster related variables
	static const UINT							c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

	ID3D12DescriptorHeap*						m_cbvHeap;
	UINT										m_cbvHeapSize;
	UINT										m_cbvHeapAllocated;

	ID3D12Resource*								m_constantBuffer;
	ModelViewProjectionConstantBuffer			m_constantBufferData;
	UINT8*										m_mappedConstantBuffer;

	DxShaderProgram*							m_shaderProgram;

	// Rasterization related functions
	bool InitializeRaster(int, int, HWND);
	void CreateRasterWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	bool RenderRaster();

	// Culling
	bool aabbOutsideFrustum(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 maxVert, std::vector<DirectX::XMFLOAT4> planes);
	bool aabbBehindPlane(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 maxVert, DirectX::XMFLOAT4 plane);

	// DXR related variables

	union AlignedSceneConstantBuffer
	{
		SceneConstantBuffer constants;
		uint8_t alignmentPadding[D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT];
	};

	union AlignedCubeConstantBuffer
	{
		CubeConstantBuffer constants;
		uint8_t alignmentPadding[D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT];
	};

	// Raytracing scene
	RayGenConstantBuffer						m_rayGenCB; /// Nvidia
	SceneConstantBuffer							m_sceneCB[c_frameCount]; /// Microsoft
	AlignedSceneConstantBuffer*					m_dxrmappedConstantBuffer;
	CubeConstantBuffer							m_cubeCB; /// Microsoft
	AlignedCubeConstantBuffer*					m_cubemappedConstantBuffer;

	// Raytracing constant resources
	ID3D12Resource*								m_dxrconstantBuffer;
	ID3D12Resource*								m_cubeConstantBuffer;

	// Acceleration structures
	std::vector<ID3D12Resource*>				m_bottomLevelAccelerationStructure;
	ID3D12Resource*								instanceDescs;
	ID3D12Resource*								m_topLevelAccelerationStructure;
	ID3D12Resource*								tscratchResource;

	// Ray tracing pipeline state object and properties
	DXRGlobal									dxr;
	ID3D12StateObject*							m_rtStateObject;
	ID3D12StateObjectProperties*				m_rtStateObjectProps;

	// Raytracing output
	ID3D12Resource*								m_raytracingOutput;

	// Shader tables
	ID3D12Resource*								m_shaderTable = nullptr;
	uint32_t									m_shaderTableRecordSize = 0;
	
	// Raytracing related functions
	bool InitializeRaytrace(int, int, HWND);
	void CreateRaytracingWindowSizeDependentResources(int screenHeight, int screenWidth, Camera* camera);
	bool CreateRaytracingInterfaces(int screenHeight, int screenWidth, HWND hwnd);
	bool BuildBottomLevelAccelerationStructures();
	bool BuildTopLevelAccelerationStructures(bool update);
	bool SerializeAndCreateRaytracingRootSignature(D3D12_ROOT_SIGNATURE_DESC& desc, ID3D12RootSignature** rootSig);
	bool CreateRaytracingPipelineStateObject();
	bool CreateDescriptorHeap();
	bool CreateDescHeapViews();
	bool BuildShaderTables();
	bool RenderRaytrace();
	bool DoRaytracing();
};

#endif // !_RENDERER_H_