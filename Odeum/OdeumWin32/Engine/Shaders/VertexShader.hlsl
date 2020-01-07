cbuffer MVPConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
}

// TYPEDEFS
struct VS_INPUT
{
	float3 position : POSITION;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
};

// Vertex Shader
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT Output;
	float4 pos = float4(input.position, 1.0f);
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	Output.position = pos;

	return Output;
}