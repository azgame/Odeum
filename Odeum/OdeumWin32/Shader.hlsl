// TYPEDEFS
struct VS_INPUT
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// Vertex Shader
PS_INPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT Output;
	float4 pos = float4(4, input.position, 1.0f);
	Output.Position = pos;
	Output.Color = float4(input.color, 1.0f);

	return Output;
}

// Pixel Shader
float4 ps_main(PS_INPUT In)
{
	return float4(In.color, 1);
}