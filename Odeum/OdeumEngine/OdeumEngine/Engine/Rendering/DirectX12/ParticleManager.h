#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "D3DIncludes.h"
#include "PipelineState.h"
#include "ParticleEffect.h"
#include "CommandContext.h"
#include "Buffers/ReadbackBuffer.h"

class GraphicsPSO;
class RootSignature;
class Camera;

__declspec(align(16)) struct ParticleSharedConstantBuffer
{
	Matrix4 viewProjMatrix;
	Matrix4 invView;
};

class ParticleManager
{
public:

	ParticleManager();
	~ParticleManager();

	static ParticleManager& Get() 
	{ 
		if (sm_instance == nullptr)
			sm_instance = new ParticleManager();

		return *sm_instance; 
	}

	void Initialize(uint32_t Width, uint32_t Height);
	void CreateEffect(ParticleInitProperties Properties);
	void Update(ComputeContext& Compute, float deltaTime);
	void Render(CommandContext& Context, Camera& Camera, ColourBuffer& renderTarget, DepthBuffer& depthTarget);
	void Uninitialize();

	ComputePSO GetParticleSpawnPSO() { return sm_particleSpawnCS; }
	ComputePSO GetParticleUpdatePSO() { return sm_particleUpdateCS; }
	ComputePSO GetParticleDispatchIndArgsPSO() { return sm_particleDispatchIndirectArgsCS; }
	StructuredBuffer GetParticleOutputBuffer() { return m_vertexBuffer; }

	uint32_t GetTotalNumParticles() { return *particleCount; }

private:

	static ParticleManager* sm_instance;
	static std::mutex sm_mutex;
	static std::vector<ParticleEffect*> sm_activeEffects;

	// particle manager pipelines
	ComputePSO sm_particleFinalDispatchIndirectArgsCS;
	GraphicsPSO sm_drawParticlesNoTileNoSort;

	// particle manager resources
	RootSignature m_rootSignature;
	StructuredBuffer m_indexBuffer;
	StructuredBuffer m_vertexBuffer;
	ByteAddressBuffer m_drawIndirectArgs;
	ByteAddressBuffer m_particleFinalDispatchIndirectArgs;

	ReadbackBuffer readBack;
	UINT* particleCount;

	ParticleSharedConstantBuffer m_CBperFrameData;
	
	// individual particle pipelines
	ComputePSO sm_particleSpawnCS;
	ComputePSO sm_particleUpdateCS;
	ComputePSO sm_particleDispatchIndirectArgsCS;
};

#endif