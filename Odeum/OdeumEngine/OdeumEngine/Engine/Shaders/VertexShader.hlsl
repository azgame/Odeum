cbuffer VSConstants : register(b0)
{
	matrix viewProj;
	matrix model;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float4 colour : COLOUR;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 colour : Colour;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;

	float4 pos = float4(input.position, 1.0f);
	pos = mul(pos, model);
	pos = mul(viewProj, pos);

	output.position = pos;
	output.colour = input.colour;

	return output;
}