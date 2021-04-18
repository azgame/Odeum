#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include "D3DIncludes.h"

#include "Buffers/D3DBuffer.h"
#include "Buffers/ReadbackBuffer.h"
#include "ParticleStructs.h"

class ComputeContext;

struct ParticleProperties
{
	ParticleProperties()
	{
		ZeroMemory(this, sizeof(*this));
		startColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		endColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		minLife = 1.0f;
		maxLife = 2.0f;
		lifeTime = 20.0f;
		minMass = 0.1f;
		maxMass = 1.0f;
		spread = Vector3(1.0f, 1.0f, 1.0f);
		minVelocity = Vector2(0.25f, 2.0f);
		maxVelocity = Vector2(-0.25f, 2.0f);
	}

	// Particle props
	DirectX::XMFLOAT4 startColour;
	DirectX::XMFLOAT4 endColour;
	float minLife;
	float maxLife;
	float lifeTime;

	// Physical props
	float minMass;
	float maxMass;
	float rotationMax;
	Vector3 spread;
	Vector2 minVelocity;
	Vector2 maxVelocity;
	Vector2 minSize;
	Vector2 maxSize;
	ParticleLaunchingData lauchingData;


};

class ParticleEffect
{
public:

	ParticleEffect(ParticleProperties Properties)
	{
		m_currentParticleBuffer = 0;
		elapsedTime = 0.0f;
		m_properties = Properties;
		kMaxParticles = Properties.lauchingData.maxParticles;
	}

	~ParticleEffect()
	{
		DestroyParticle();
	}

	void Initialize();
	void Update(ComputeContext& Compute, float deltaTime);

	float GetElapsedTime() { return elapsedTime; }
	float GetLifeTime() { return m_properties.lifeTime; }

	// Texture
	// std::string textureName;

	Vector3 GetPosition() { return DirectX::XMLoadFloat3(&m_properties.lauchingData.launchPosition); }
	void SetPosition(Vector3 Position) { DirectX::XMStoreFloat3(&m_properties.lauchingData.launchPosition, Position); }
	void Translate(Vector3 Position) { SetPosition(GetPosition() + Position); }
	void SetSpawnRate(float SpawnRate) { m_properties.lauchingData.spawnRate = SpawnRate; }
	void ModifySpawnRate(float ChangeRate) { m_properties.lauchingData.spawnRate += ChangeRate; }

private:

	void DestroyParticle();
	
	uint32_t kMaxParticles = 1000;
	float elapsedTime;

	ParticleProperties m_properties;

	uint8_t m_currentParticleBuffer;
	StructuredBuffer m_particleBuffer[2];
	StructuredBuffer m_spawnStateBuffer;
	ByteAddressBuffer m_dispatchArgsBuffer; 
	// D3D12_CPU_DESCRIPTOR_HANDLE m_particleTexture;

	ReadbackBuffer particleCountReadback;
	UINT* particleCount;
	ReadbackBuffer threadGroupReadback;
	UINT* threadGroupCount;
};

#endif