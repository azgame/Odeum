struct PS_INPUT
{
	float4 position : SV_POSITION;
};

// Pixel Shader
float4 main(PS_INPUT In) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}