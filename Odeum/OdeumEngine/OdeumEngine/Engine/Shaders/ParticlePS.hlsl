#include "ParticleCommon.hlsli"

float4 main(ParticleVertexOutput input) : SV_TARGET0
{
	return input.Colour;
}