#ifndef PARTICLESTRUCTS_H
#define PARTICLESTRUCTS_H

#include "Colour.h"
#include "../../Math/D3DMath.h"
#include "Buffers/D3DBuffer.h"

struct ParticleSpawnData
{
	ParticleSpawnData() : colour(1.0f, 1.0f, 1.0f)
	{
		ZeroMemory(this, sizeof(*this));
		ageSpeed = 0.0f;
		startingVelocity = Vector3(kZero);
		mass = 0.0f;
		rotation = 0.0f;
		startSize = 0.0f;
		endSize = 0.0f;
		spread = Vector3(kZero);
	}

	float ageSpeed;
	Vector3 startingVelocity;
	float mass;
	float rotation;
	float startSize;
	float endSize;
	Vector3 spread;
	Colour colour;
};

__declspec(align(16)) struct ParticleLaunchingData
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	float speed;
	UINT maxParticles;
	float spawnRate;
	float pad1;
	float pad2;
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
	Vector3 position;
	Colour colour;
	float size;
};

#endif