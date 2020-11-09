#include "ParticleCommon.hlsli"

StructuredBuffer<ParticleVertex> vertexBuffer : register(t1);
StructuredBuffer<uint> indexBuffer : register(t2);

[RootSignature(Particle_RootSig)]
ParticleVertexOutput main( uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID )
{
	ParticleVertex In = vertexBuffer[instanceID];
	ParticleVertexOutput Out;

	Out.TexCoord = float2((vertexID >> 1) & 1, vertexID & 1);
	Out.Colour = In.colour;

	float2 Corner = lerp(float2(-1, 1), float2(1, -1), Out.TexCoord);
	float3 Position = mul((float3x3)invView, float3(Corner * In.size, 0)) + In.position;

	Out.Pos = mul(viewProj, float4(Position, 1.0f));
	Out.LinearZ = Out.Pos.w * farClipZ;
	return Out;
}