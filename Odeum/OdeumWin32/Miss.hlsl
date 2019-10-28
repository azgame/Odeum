#ifndef MISS_HLSL
#define MISS_HLSL

#include "Common.hlsl"

[shader("miss")]
void MyMissShader(inout RayPayload payload)
{
	payload.color = float4(0, 0, 0, 1);
}

#endif