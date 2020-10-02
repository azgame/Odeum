#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "D3DIncludes.h"

_declspec(align(16)) struct LightData
{
	DirectX::XMFLOAT3 position;
	float radiusSq;
	DirectX::XMFLOAT3 colour;
};

class LightSource
{
};

#endif