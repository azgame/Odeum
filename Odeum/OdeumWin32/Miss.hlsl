#include "Common.hlsl"

[shader("miss")]
void MyMissShader(inout RayPayload payload)
{
	uint2 launchIndex = DispatchRaysIndex();
	float2 dims = float2(DispatchRaysDimensions());

	float ramp = launchIndex.y / dims.y;
	payload.color = float4(0.0f, 0.2f, 0.7f - 0.3f*ramp, 1.0f);
}