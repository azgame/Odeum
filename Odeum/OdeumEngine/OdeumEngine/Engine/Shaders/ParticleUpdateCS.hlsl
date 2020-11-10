#include "ParticleCommon.hlsli"
#include "ParticleUpdateUtility.hlsli"

cbuffer CBO : register(b0)
{
	float elapsedTime;
};

StructuredBuffer<ParticleSpawnData> spawnData : register(t0);
StructuredBuffer<ParticleSimulationData> inputBuffer : register(t1);
RWStructuredBuffer<ParticleVertex> vertexBuffer : register(u0);
RWStructuredBuffer<ParticleSimulationData> outputBuffer : register(u2);

[RootSignature(Particle_RootSig)]
[numthreads(64, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= maxParticles)
		return;

	ParticleSimulationData particle = inputBuffer[DTid.x];
	ParticleSpawnData spawn = spawnData[particle.spawnIndex];

	particle.age += elapsedTime * spawn.ageSpeed;
	if (particle.age >= 1.0)
		return;

	float stepSize = (particle.position.y > 0.0 && particle.velocity.y < 0.0) ?
		min(elapsedTime, particle.position.y / -particle.velocity.y) : elapsedTime;

	particle.position += particle.velocity * stepSize;
	particle.velocity += gravity * particle.mass * stepSize;

	stepSize = elapsedTime - stepSize;
	if (stepSize > 0.0)
	{
		particle.velocity = reflect(particle.velocity, float3(0, 1, 0)) * groundBounce;
		particle.position += particle.velocity * stepSize;
		particle.velocity += gravity * particle.mass * stepSize;
	}

	uint index = outputBuffer.IncrementCounter();
	if (index >= maxParticles)
		return;

	outputBuffer[index] = particle;

	ParticleVertex vertex;
	vertex.position = particle.position;
	vertex.size = lerp(spawn.startSize, spawn.endSize, particle.age);
	vertex.colour = lerp(spawn.startColour, spawn.endColour, particle.age);
	vertex.colour *= particle.age * (1.0 - particle.age) * (1.0 - particle.age) * 6.7;

	vertexBuffer[vertexBuffer.IncrementCounter()] = vertex;
}