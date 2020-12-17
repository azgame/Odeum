#include "ParticleRS.hlsli"

cbuffer ChangesPerFrame : register(b1)
{
	float4x4 viewProj;
	float4x4 invView;
};

struct ParticleVertex
{
    float3 position;
    float size;
    float4 colour;
};