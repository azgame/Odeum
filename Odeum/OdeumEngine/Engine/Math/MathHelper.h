#ifndef MATHHELPER_H
#define MATHHELPER_H

#include "../../pch.h"

DirectX::XMFLOAT3 Float4To3(DirectX::XMFLOAT4 float4)
{
	DirectX::XMFLOAT3 float3;
	float3.x = float4.x;
	float3.y = float4.y;
	float3.z = float4.z;
	return float3;
}

DirectX::XMFLOAT4 Float3To4(DirectX::XMFLOAT3 float3)
{
	DirectX::XMFLOAT4 float4;
	float4.x = float3.x;
	float4.y = float3.y;
	float4.z = float3.z;
	float4.w = 1.0f;
	return float4;
}

#endif