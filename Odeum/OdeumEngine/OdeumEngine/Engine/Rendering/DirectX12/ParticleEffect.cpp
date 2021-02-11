#include "ParticleEffect.h"

#include "CommandContext.h"
#include "D3DCore.h"

#include "../../Math/MathUtility.h"

#include "ParticleManager.h"

// Comments by Aidan Zizys, 2020

void ParticleEffect::Initialize()
{
	// create a vector of spawn data to be passed into the spawn particle buffer
	std::vector<ParticleSpawnData> spawnData(kMaxParticles);

	// For each particle, fill an entry in the spawn data with randomized data, within the ranges provided by the particle effect data
	for (uint32_t i = 0; i < kMaxParticles; i++)
	{
		spawnData[i].ageSpeed = 1.0f / Math::RandValInRange(m_properties.minLife, m_properties.maxLife);
		float horizontalAngle = Math::RandValInRange(0.0f, DirectX::XM_2PI);
		float horizontalVelocity = Math::RandValInRange(m_properties.minVelocity.GetX(), m_properties.minVelocity.GetY());
		spawnData[i].startingVelocity.x = horizontalVelocity * cos(horizontalAngle);
		spawnData[i].startingVelocity.y = Math::RandValInRange(m_properties.maxVelocity.GetX(), m_properties.maxVelocity.GetY());
		spawnData[i].startingVelocity.z = horizontalVelocity * sin(horizontalAngle);
		DirectX::XMStoreFloat3(&spawnData[i].spread, m_properties.spread);
		spawnData[i].startSize = Math::RandValInRange(m_properties.minSize.GetX(), m_properties.minSize.GetY());
		spawnData[i].endSize = Math::RandValInRange(m_properties.maxSize.GetX(), m_properties.maxSize.GetY());
		spawnData[i].startColour = m_properties.startColour;
		spawnData[i].endColour = m_properties.endColour;
		spawnData[i].mass = Math::RandValInRange(m_properties.minMass, m_properties.maxMass);
		spawnData[i].rotation = Math::RandValInRange(0.0f, m_properties.rotationMax);
	}

	// Create appropriate buffers (spawn buffer, 2 particle buffers, dispatch args buffer)
	m_spawnStateBuffer.Create("SpawnDataBuffer", kMaxParticles, sizeof(ParticleSpawnData), spawnData.data());

	m_particleBuffer[0].Create("Particle Buffer", kMaxParticles, sizeof(ParticleSimulationData));
	m_particleBuffer[1].Create("Particle Buffer", kMaxParticles, sizeof(ParticleSimulationData));

	__declspec(align(16)) UINT dispatchIndirectData[3] = { 0, 1, 1 };
	m_dispatchArgsBuffer.Create("Dispatch Args Buffer", 1, sizeof(D3D12_DISPATCH_ARGUMENTS), dispatchIndirectData);

	particleCountReadback.Create(1, 4);
	threadGroupReadback.Create(1, 4);
}

void ParticleEffect::Update(ComputeContext& Compute, float deltaTime)
{
	elapsedTime += deltaTime;

	// Get 64 random particle indices (between 0 and kMaxParticles) to determine the pre generated particle spawn details to be used
	for (int i = 0; i < 64; i++)
		m_properties.lauchingData.randomIndices[i].x = Math::RandIntInRange(0, kMaxParticles - 1);

	/* 
		Two particle buffers, one spawn buffer. When particle is spawned, its state (position, velocity, etc.) is written to 1st buffer.
		In the next pass, the state is retrieved from 1st buffer, updated, and written to 2nd buffer. 1st buffer is transitioned so
		that it can be used to write to. Spawning particles are written to 2nd buffer. Current state of particles is in 2nd buffer.
		Process repeats for particles until they have exceeded their lifetime, and are removed, opening space for new particles to spawn.
	*/

	// Pre fill the cbv with particle launching details (ie. spawn location, initial direction, speed, max particles, etc.)
	// Transition the currently used particle buffer to a shader resource to get the state of each alive particle
	// Set current particle buffer and spawn buffer as srvs
	Compute.SetDynamicConstantBufferView(2, sizeof(ParticleLaunchingData), &m_properties.lauchingData);

	Compute.TransitionResource(m_particleBuffer[m_currentParticleBuffer], D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	Compute.SetDynamicDescriptor(4, 0, m_spawnStateBuffer.ShaderResourceView());
	Compute.SetDynamicDescriptor(4, 1, m_particleBuffer[m_currentParticleBuffer].ShaderResourceView());

	m_currentParticleBuffer ^= 1; // xor 1 to flip between 1 and 0 each frame

	// Reset the counter (alive particles) of the other particle buffer (which will become the current particle buffer)
	Compute.ResetCounter(m_particleBuffer[m_currentParticleBuffer]);

	// Set to update pso
	// Transition the other buffer (the next alive buffer) to uav for concurrent read/write
	// Transition our indirect dispatch args buffer to an args state so it can be read as the dispatch args in our indirect execute
	// Set the other buffer (next alive buffer) as a uav descriptor so it can be written to
	Compute.SetPipelineState(ParticleManager::Get().GetParticleUpdatePSO());
	Compute.TransitionResource(m_particleBuffer[m_currentParticleBuffer], D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	Compute.TransitionResource(m_dispatchArgsBuffer, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);
	Compute.SetDynamicDescriptor(3, 2, m_particleBuffer[m_currentParticleBuffer].UnorderedAccessView());
	Compute.DispatchIndirect(m_dispatchArgsBuffer, 0);

	// Barrier so that living particles take precedent over spawning new particles
	Compute.InsertUAVBarrier(m_particleBuffer[m_currentParticleBuffer]);

	// Set to spawn pso
	// Set spawn buffer as srv for reading
	// Number of spawn threads is how fast we should be spawning new particles each second. numSpawnThreads will determine how many
	//	batches of 64 particles we will spawn this frame, up to kMaxParticles.
	Compute.SetPipelineState(ParticleManager::Get().GetParticleSpawnPSO());
	Compute.SetDynamicDescriptor(4, 0, m_spawnStateBuffer.ShaderResourceView());
	UINT numSpawnThreads = (UINT)(m_properties.lauchingData.spawnRate * deltaTime);
	Compute.Dispatch((numSpawnThreads + 63) / 64, 1, 1);

	// Set to finding dispatch args pso
	// Goal here is to write the number of particles (divided by 64) to the dispatch args buffer so that when we update our particles,
	//	we run the appropriate number of thread groups (in the compute shader it is multiplied by 64, so 64 threads per group, totaling # of particles)
	Compute.SetPipelineState(ParticleManager::Get().GetParticleDispatchIndArgsPSO());
	Compute.TransitionResource(m_dispatchArgsBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	Compute.TransitionResource(m_particleBuffer[m_currentParticleBuffer], D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	Compute.SetDynamicDescriptor(4, 0, m_particleBuffer[m_currentParticleBuffer].GetCounterSRV(Compute));
	Compute.SetDynamicDescriptor(3, 1, m_dispatchArgsBuffer.UnorderedAccessView());
	Compute.Dispatch(1, 1, 1);
}