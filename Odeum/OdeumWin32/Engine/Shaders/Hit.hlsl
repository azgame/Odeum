#include "Common.hlsl"

[shader("closesthit")]
void MyClosestHitShader(inout RayPayload payload, in MyAttributes attr)
{
	float3 hitPosition = HitWorldPosition();

	// Get the base index of the triangle's first 16 bit index
	uint indexSizeInBytes = 2;
	uint indicesPerTriangle = 3;
	uint triangleIndexStride = indicesPerTriangle * indexSizeInBytes;
	uint baseIndex = PrimitiveIndex() * triangleIndexStride;

	// Load up 3 16 bit indices for the triangle
	const uint3 indices = Load3x16BitIndices(baseIndex);

	// Retrieve corresponding vertex normals for the triangle vertices
	float3 vertexNormals[3] = {
		Vertices[indices[0]].normal,
		Vertices[indices[1]].normal,
		Vertices[indices[2]].normal
	};

	// Compute the triangle's normal
	float3 triangleNormal = HitAttribute(vertexNormals, attr);

	float4 diffuseColor = CalculateDiffuseLighting(hitPosition, triangleNormal);
	float4 color = g_sceneCB.lightAmbient + diffuseColor;

	payload.color = color;
}