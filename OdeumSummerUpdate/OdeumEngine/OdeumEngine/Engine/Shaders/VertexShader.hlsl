cbuffer VSConstants : register(b0)
{
	matrix viewProj;
	matrix model;
};

struct VS_INPUT
{
	float3 position : POSITION;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 worldPos : WorldPos;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;

	float4 pos = float4(input.position, 1.0f);
	output.worldPos = pos;
	pos = mul(pos, model);
	pos = mul(viewProj, pos);

	output.position = pos;

	return output;
}