#include "Commonold.hlsl"

[shader("closesthit")]
void MyClosestHitShader(inout HitInfo payload, in Attributes attr)
{
	float3 barycentrics = float3(1 - attr.barycentrics.x - attr.barycentrics.y, attr.barycentrics.x, attr.barycentrics.y);
	payload.color = float4(barycentrics, 1);
}