#ifndef RAYGEN_HLSL
#define RAYGEN_HLSL

#include "Common.hlsl"

[shader("raygeneration")]
void MyRaygenShader()
{
	float2 lerpValues = (float2)DispatchRaysIndex() / (float2)DispatchRaysDimensions();

	// Orthographic projection since we're raytracing in screen space.
	float3 rayDir = float3(0, 0, 1);
	float3 origin = float3(
		lerp(g_rayGenCB.viewport.left, g_rayGenCB.viewport.right, lerpValues.x),
		lerp(g_rayGenCB.viewport.top, g_rayGenCB.viewport.bottom, lerpValues.y),
		0.0f);

	if (IsInsideViewport(origin.xy, g_rayGenCB.stencil))
	{
		// Trace the ray.
		// Set the ray's extents.
		RayDesc ray;
		ray.Origin = origin;
		ray.Direction = rayDir;
		// Set TMin to a non-zero small value to avoid aliasing issues due to floating - point errors.
		// TMin should be kept small to prevent missing geometry at close contact areas.
		ray.TMin = 0.001;
		ray.TMax = 10000.0;
		RayPayload payload = { float4(0, 0, 0, 0) };
		TraceRay(Scene, RAY_FLAG_CULL_BACK_FACING_TRIANGLES, ~0, 0, 1, 0, ray, payload);

		// Write the raytraced color to the output texture.
		RenderTarget[DispatchRaysIndex().xy] = payload.color;
	}
	else
	{
		// Render interpolated DispatchRaysIndex outside the stencil window
		RenderTarget[DispatchRaysIndex().xy] = float4(lerpValues, 0, 1);
	}
}


#endif