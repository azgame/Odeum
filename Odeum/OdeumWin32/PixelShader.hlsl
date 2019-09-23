struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// Pixel Shader
float4 main(PS_INPUT In) : SV_TARGET
{
	return float4(1.0, 0.0, 0.0, 1.0);
}