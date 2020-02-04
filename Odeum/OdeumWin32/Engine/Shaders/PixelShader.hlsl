struct PS_INPUT
{
	float4 position : SV_POSITION;
};

// Pixel Shader
float4 main(PS_INPUT In) : SV_TARGET
{
	return normalize(In.position);
}