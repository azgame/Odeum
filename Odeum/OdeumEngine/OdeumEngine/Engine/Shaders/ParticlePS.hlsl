#include "ParticleCommon.hlsli"
#include "ParticleUpdateUtility.hlsli"

[RootSignature(Particle_RootSig)]
float4 main(ParticleVertexOutput input) : SV_TARGET0
{
	return input.Colour;
}