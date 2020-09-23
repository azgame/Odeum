
Texture2D<float3> colourTexture : register(t0);

float3 main(float4 position : SV_Position) : SV_Target0
{
	float3 colour = colourTexture[(int2)position.xy];
	return colour;
}