struct VS_INPUT
{
	float3 position : POSITION;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.position = float4(input.position, 1.0f);
	return output;
}