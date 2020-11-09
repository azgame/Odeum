#ifndef PARTICLESTRUCTS_H
#define PARTICLESTRUCTS_H

#include "Colour.h"
#include "../../Math/D3DMath.h"
#include "Buffers/D3DBuffer.h"

struct ParticleSpawnData
{
	ParticleSpawnData()
	{
		ZeroMemory(this, sizeof(*this));
		ageSpeed = 0.0f;
		startingVelocity = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		mass = 0.0f;
		rotation = 0.0f;
		startSize = 0.0f;
		endSize = 0.0f;
		spread = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		colour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float ageSpeed;
	DirectX::XMFLOAT3 startingVelocity;
	float mass;
	float rotation;
	float startSize;
	float endSize;
	DirectX::XMFLOAT3 spread;
	DirectX::XMFLOAT4 colour;
};

__declspec(align(16)) struct ParticleLaunchingData
{
	DirectX::XMFLOAT3 launchPosition;
	DirectX::XMFLOAT3 xAxis;
	DirectX::XMFLOAT3 yAxis;
	DirectX::XMFLOAT3 zAxis;
	float speed;
	UINT maxParticles;
	float spawnRate;
	DirectX::XMFLOAT3 gravity;
	float groundBounce;
	UINT randomIndices[64];
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
	DirectX::XMFLOAT4 colour;
	float size;
};

#endif