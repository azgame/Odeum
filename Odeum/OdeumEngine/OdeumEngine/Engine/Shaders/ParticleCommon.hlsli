
cbuffer LaunchData : register(b2)
{
	float3 launchPosition;
	float speed;
	float3 xAxis;
	uint maxParticles;
	float3 yAxis;
	float spawnRate;
	float3 zAxis;
	float groundBounce;
	float3 gravity;
	float pad1;
	uint4 randomIndices[64];
};

struct ParticleSpawnData
{
	float ageSpeed;
	float rotation;
	float mass;
	float startSize;
	float3 startingVelocity;
	float endSize;
	float3 spread;
	float pad2;
	float4 startColour;
	float4 endColour;
};

struct ParticleSimulationData
{
	float3 position;
	float mass;
	float3 velocity;
	float age;
	float rotation;
	uint spawnIndex;
};

struct ParticleVertexOutput
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Colour : TEXCOORD1;
};