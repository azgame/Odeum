#include "AnimationTestScene.h"

#include "../../Engine/Math/CollisionHandler.h"
#include "../../Engine/Math/Collider.h"
#include "../../Engine/Math/Plane.h"
#include "../../Engine/NavMesh/NavMeshManager.h"
#include "../../Engine/Rendering/DirectX12/ParticleManager.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../../Engine/CoreComponents/AudioSource.h"
#include "../../Engine/Core/AudioHandler.h"

AnimationTestScene::AnimationTestScene() : Scene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	go1 = new GameObject();
	go1->AddComponent<RenderComponent>();
	go1->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Pig.obj");
	go1->AddComponent<Rigidbody>();
	go1->GetComponent<Rigidbody>()->SetPosition(Vector4(-2.0f, 0.0f, 0.0f, 1.0f));

	go2 = new GameObject();
	go2->AddComponent<RenderComponent>();
	go2->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/BaseCharacter.fbx", true);
	go2->AddComponent<Rigidbody>();
	go2->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, 0.0f, 0.0f, 1.0f));
	go2->GetComponent<Rigidbody>()->AddRotation(Vector4(0.0f, 1.0f, 0.0f, 1.0f), 180.0f);
	go2->GetComponent<Rigidbody>()->AddRotation(Vector4(1.0f, 0.0f, 0.0f, 1.0f), -90.0f);

	ParticleProperties particleProps;
	particleProps.startColour = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	particleProps.endColour = DirectX::XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f);
	particleProps.lifeTime = FLT_MAX;
	particleProps.minLife = 4.0f;
	particleProps.maxLife = 8.0f;
	particleProps.minMass = 0.1f;
	particleProps.maxMass = 0.4f;
	particleProps.rotationMax = 1.0f;
	particleProps.spread = Vector3(20.0f, 50.0f, 20.0f);
	particleProps.minVelocity = Vector2(0.4f, 0.6f);
	particleProps.maxVelocity = Vector2(0.3f, 0.4f);
	particleProps.minSize = Vector2(0.0050f, 0.0075f);
	particleProps.maxSize = Vector2(0.01f, 0.025f);
	DirectX::XMStoreFloat3(&particleProps.lauchingData.xAxis, Vector3(kXUnitVector));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.yAxis, Vector3(kYUnitVector));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.zAxis, Vector3(kZUnitVector));
	particleProps.lauchingData.maxParticles = 5000;
	particleProps.lauchingData.spawnRate = 64.0f;
	particleProps.lauchingData.speed = 1.0f;
	particleProps.lauchingData.groundBounce = 1.02f;
	DirectX::XMStoreFloat3(&particleProps.lauchingData.gravity, Vector3(0.0f, -0.1f, 0.0f));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.launchPosition, Vector3(0.0f, 0.0f, -2.0f));
	// particleProps.textureID --

	particle = &ParticleManager::Get().CreateEffect(particleProps);

	SceneGraph::Get()->LoadGraphicsObjects();
}

AnimationTestScene::~AnimationTestScene()
{
}

bool AnimationTestScene::Initialize()
{
	LOG("Creating Game Scene")
		return true;
}

void AnimationTestScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();
	
	particle->Translate(Vector3(0.02, 0.0, 0.0));
}

void AnimationTestScene::UIRender()
{

}
