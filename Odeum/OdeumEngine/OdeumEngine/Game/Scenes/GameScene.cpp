#include "GameScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../../Engine/Math/CollisionHandler.h"
#include "../Components/Rigidbody.h"
#include "../Engine/Math/Collider.h"
#include "../Engine/Math/Plane.h"

#include "../../Engine/Rendering/DirectX12/ParticleManager.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/NavMesh/NavMeshManager.h"

GameScene::GameScene() : Scene(), angle(0.0f), direction(1.0f)
{
	object = new GameObject("Engine/Resources/Models/Cottage_FREE.obj");
	object->AddComponent<Rigidbody>();
	object->GetComponent<Rigidbody>()->SetPosition(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	/*ParticleInitProperties particleProps;
	particleProps.startColour = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	particleProps.endColour = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	particleProps.lifeTime = FLT_MAX;
	particleProps.minLife = 10.0f;
	particleProps.maxLife = 25.0f;
	particleProps.minMass = 0.1f;
	particleProps.maxMass = 0.4f;
	particleProps.rotationMax = 1.0f;
	particleProps.spread = Vector3(20.0f, 50.0f, 20.0f);
	particleProps.minVelocity = Vector2(0.4f, 0.6f);
	particleProps.maxVelocity = Vector2(0.3f, 0.4f);
	particleProps.minSize = Vector2(0.0050f, 0.0075f);
	particleProps.maxSize = Vector2(0.01f, 0.0125f);
	DirectX::XMStoreFloat3(&particleProps.lauchingData.xAxis, Vector3(kXUnitVector));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.yAxis, Vector3(kYUnitVector));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.zAxis, Vector3(kZUnitVector));
	particleProps.lauchingData.maxParticles = 100000;
	particleProps.lauchingData.spawnRate = 64.0f;
	particleProps.lauchingData.speed = 1.0f;
	particleProps.lauchingData.groundBounce = 1.02f;
	DirectX::XMStoreFloat3(&particleProps.lauchingData.gravity, Vector3(0.0f, -0.1f, 0.0f));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.launchPosition, Vector3(0.0f, 0.0f, -10.0f));

	ParticleManager::Get().CreateEffect(particleProps);*/

	NavMeshManager::Initialize();

	std::vector<Vector2> t1;
	t1.push_back(Vector2(4.0f, 5.0f));
	t1.push_back(Vector2(4.0f, 11.0f));
	t1.push_back(Vector2(9.0f, 9.0f));

	std::vector<Vector2> t2;
	t2.push_back(Vector2(5.0f, 7.0f));
	t2.push_back(Vector2(12.0f, 7.0f));
	t2.push_back(Vector2(10.0f, 2.0f));
	t2.push_back(Vector2(7.0f, 3.0f));

	std::vector<Collider2D> obstacles = { Collider2D(t1), Collider2D(t2) };

	Plane groundPlane;
	groundPlane.min = Vector3(-20.0f, 0.0f, -20.0f);
	groundPlane.max = Vector3(20.0f, 0.0f, 20.0f);
	groundPlane.normal = Vector3(kYUnitVector);

	NavMeshManager::GenerateNavMesh(1, groundPlane, obstacles);

	SceneGraph::Get()->LoadObjectsIntoMemory();
}

GameScene::~GameScene()
{
}

bool GameScene::Initialize()
{
	LOG("Creating Game Scene")
	return true;
}

void GameScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	SceneGraph::Get()->UpdateObjects(deltaTime_);
}

void GameScene::UIRender()
{
}
