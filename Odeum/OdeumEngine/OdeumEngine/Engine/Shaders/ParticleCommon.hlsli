#include "ParticleRS.hlsli"

cbuffer ChangesPerFrame : register(b1)
{
    float4x4 viewProj;
	float4x4 invView;
    float aspectRatio;
	float farClipZ;
};

cbuffer LaunchData : register(b2)
{
	float3 launchPosition;
	float3 xAxis;
	float3 yAxis;
	float3 zAxis;
	float speed;
	uint maxParticles;
	float spawnRate;
	float3 gravity;
	float groundBounce;
	uint randomIndices[64];
};

struct ParticleSpawnData
{
	float ageSpeed;
	float3 startingVelocity;
	float mass;
	float rotation;
	float startSize;
	float endSize;
	float3 spread;
	float4 colour;
};

struct ParticleSimulationData
{
	float3 position;
	float3 velocity;
	float mass;
	float age;
	float rotation;
	uint spawnIndex;
};

struct ParticleVertex
{
    float3 position;
    float4 colour;
    float size;
};

struct ParticleVertexOutput
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	nointerpolation float4 Colour : TEXCOORD2;
	nointerpolation float LinearZ : TEXCOORD3;
};