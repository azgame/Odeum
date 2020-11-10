#include "ParticleCommon.hlsli"
#include "ParticleUpdateUtility.hlsli"

StructuredBuffer<ParticleSpawnData> spawnData : register(t0);
RWStructuredBuffer<ParticleSimulationData> outputBuffer : register(u2);

[RootSignature(Particle_RootSig)]
[numthreads(64, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	uint index = outputBuffer.IncrementCounter();
	if (index >= maxParticles)
		return;

	uint idx = DTid.x;
	uint particleIndex = randomIndices[idx].x;
	ParticleSpawnData spawn = spawnData[particleIndex];

	float3 launchVelocity = spawn.startingVelocity.x * xAxis + spawn.startingVelocity.y * yAxis + spawn.startingVelocity.z * zAxis;
	float3 startPos = launchPosition;

	ParticleSimulationData particle;
	particle.position = startPos;
	particle.velocity = launchVelocity;
	particle.mass = spawn.mass;
	particle.age = 0.0;
	particle.rotation = spawn.rotation;
	particle.spawnIndex = particleIndex;
	outputBuffer[index] = particle;
}