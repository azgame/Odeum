Texture2D t1 : register(t0);
Texture2D t2 : register(t1);
SamplerState s1 : register(s0);

struct ConstantData
{
	bool constant;
};

ConstantBuffer<ConstantData> myBuffer : register(b1);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 colour : COLOUR;
	float2 uv : UV;
};

// Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
	if (myBuffer.constant)
	{
		return t1.Sample(s1, input.uv);
	}	
	else
	{
		return t2.Sample(s1, input.uv);
	}
}