#include "ParticleCommon.hlsli"
#include "ParticleUpdateUtility.hlsli"

ByteAddressBuffer instanceCounter : register(t0);
RWByteAddressBuffer numThreadGroups : register(u0);
RWByteAddressBuffer drawIndirectArgs : register(u1);

[RootSignature(Particle_RootSig)]
[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	uint particleCount = instanceCounter.Load(0);
	numThreadGroups.Store3(0, uint3((particleCount + 63) / 64, 1, 1)); // At address 0, store aligned divide particle count (thread group contains group of 64 thread)
	drawIndirectArgs.Store(4, particleCount); // At address 4 (second address), store particle count to know how many indices to draw
}