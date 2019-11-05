#include "Common.hlsl"

[shader("miss")]
void MyMissShader(inout RayPayload payload)
{
	float4 background = float4(0.0f, 0.2f, 0.4f, 1.0f);
	payload.color = background;
}