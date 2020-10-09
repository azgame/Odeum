Texture2D<float3> colourTexture : register(t0);
Texture2D<float4> overlayTexture : register(t1);

float3 main(float4 position : SV_Position) : SV_Target0
{
	float3 colour = colourTexture[(int2)position.xy];
	float4 uiColour = overlayTexture[(int2)position.xy];

	//colour = colour * (1 - uiColour.a) + uiColour.rgb;
	
	return colour;
}