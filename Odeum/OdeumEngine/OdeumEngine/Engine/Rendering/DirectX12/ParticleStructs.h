#ifndef PARTICLESTRUCTS_H
#define PARTICLESTRUCTS_H

#include "Colour.h"
#include "../../Math/D3DMath.h"
#include "Buffers/D3DBuffer.h"

__declspec(align(16)) struct ParticleSpawnData
{
	float ageSpeed;
	float rotation;
	float mass;
	float startSize;
	DirectX::XMFLOAT3 startingVelocity;
	float endSize;
	DirectX::XMFLOAT3 spread;
	float pad;
	DirectX::XMFLOAT4 startColour;
	DirectX::XMFLOAT4 endColour;
};

__declspec(align(16)) struct ParticleLaunchingData
{
	DirectX::XMFLOAT3 launchPosition;
	float speed;
	DirectX::XMFLOAT3 xAxis;
	UINT maxParticles;
	DirectX::XMFLOAT3 yAxis;
	float spawnRate;
	DirectX::XMFLOAT3 zAxis;
	float groundBounce;
	DirectX::XMFLOAT3 gravity;
	float pad;
	DirectX::XMUINT4 randomIndices[64];
};

struct ParticleSimulationData
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	float mass;
	float age;
	float rotation;
	UINT newParticleIndex;
};

struct ParticleVertexData
{
	DirectX::XMFLOAT3 position;
	float size;
	DirectX::XMFLOAT4 colour;
};

#endif