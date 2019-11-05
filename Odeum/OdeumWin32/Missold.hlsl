#include "Commonold.hlsl"

[shader("miss")]
void MyMissShader(inout HitInfo payload)
{
	payload.ShadedColorAndHitT = float4(0.2f, 0.2f, 0.2f, -1.f);
}