#include "Common.hlsl"

[shader("closesthit")]
void MyClosestHitShader(inout HitInfo payload, in Attributes attr)
{
	uint triangleIndex = PrimitiveIndex();
	float3 barycentrics = float3((1.0f - attr.uv.x - attr.uv.y), attr.uv.x, attr.uv.y);
	VertexAttributes vertex = GetVertexAttributes(triangleIndex, barycentrics);

	int2 coord = floor(vertex.uv * textureResolution.x);
	float3 color = albedo.Load(int3(coord, 0)).rgb;

	payload.ShadedColorAndHitT = float4(color, RayTCurrent());
}