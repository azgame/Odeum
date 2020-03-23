Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 colour : COLOUR;
	float2 uv : UV;
};

// Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
	return t1.Sample(s1, input.uv);
}