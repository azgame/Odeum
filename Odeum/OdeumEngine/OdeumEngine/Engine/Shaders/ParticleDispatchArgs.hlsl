#include "ParticleRS.hlsli"

ByteAddressBuffer particleInstance : register(t0);
RWByteAddressBuffer numThreadGroups : register(u1);

[RootSignature(Particle_RootSig)]
[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	numThreadGroups.Store(0, (particleInstance.Load(0) + 63) / 64);
}