cbuffer VSConstants : register(b0)
{
	matrix modelToProjection;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : Normal;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.position = mul(modelToProjection, float4(input.position, 1.0f));
	output.normal = input.normal;
	return output;
}