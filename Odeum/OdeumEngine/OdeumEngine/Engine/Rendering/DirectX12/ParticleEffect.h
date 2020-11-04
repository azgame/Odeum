#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include "D3DIncludes.h"

#include "Colour.h"
#include "../../Math/D3DMath.h"

#include "Buffers/D3DBuffer.h"

class ComputeContext;


class ParticleEffect
{
public:

	ParticleEffect() : colour(1.0f, 1.0f, 1.0f)
	{
		ZeroMemory(this, sizeof(*this));

		colour = Colour(1.0f, 1.0f, 1.0f, 1.0f);
		lifeTime = 1.0f;
		emission = 0.0f;

		scale = Vector2(1.0f, 1.0f);
		rotation = 0.0f;
		size = Vector3(1.0f, 1.0f, 1.0f);
		spread = Vector3(1.0f, 1.0f, 1.0f);
		position = Vector3(0.0f, 0.0f, 0.0f);
		velocity = Vector3(0.0f, 0.0f, 0.0f);
	}

	void Initialize();
	void Update(ComputeContext& Compute, float deltaTime);
	void Destroy();

	// Particle props
	float emission;
	Colour colour;
	float lifeTime;

	// Texture
	std::string textureName;

	// Physical props
	float mass;
	Vector2 scale;
	float rotation;
	Vector3 size;
	Vector3 spread;
	Vector3 position;
	Vector3 velocity;

private:
	
	uint32_t kMaxParticles = 1000;
	float elapsedTime;

	StructuredBuffer m_particleBuffer;
	StructuredBuffer m_spawnStateBuffer;
	ByteAddressedBuffer m_aliveList[2];
	ByteAddressedBuffer m_deadList;

	ByteAddressedBuffer m_dispatchBuffer; 
	D3D12_CPU_DESCRIPTOR_HANDLE m_randNoiseTex; // texture for generating random noise to find random values
};

#endif