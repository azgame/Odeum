#include "GameScene.h"

#include "../../Engine/Math/CollisionHandler.h"
#include "../../Engine/Math/Collider.h"
#include "../../Engine/Math/Plane.h"
#include "../../Engine/NavMesh/NavMeshManager.h"
#include "../../Engine/Rendering/DirectX12/ParticleManager.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../../Engine/CoreComponents/BoxCollider.h"
#include "../Components/StatComponent.h"

GameScene::GameScene() : Scene(), angle(0.0f), direction(1.0f)
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	// Particles
	/*ParticleInitProperties particleProps;
	particleProps.startColour = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	particleProps.endColour = DirectX::XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f);
	particleProps.lifeTime = FLT_MAX;
	particleProps.minLife = 10.0f;
	particleProps.maxLife = 20.0f;
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
	particleProps.lauchingData.maxParticles = 40000;
	particleProps.lauchingData.spawnRate = 64.0f;
	particleProps.lauchingData.speed = 1.0f;
	particleProps.lauchingData.groundBounce = 1.02f;
	DirectX::XMStoreFloat3(&particleProps.lauchingData.gravity, Vector3(0.0f, -0.1f, 0.0f));
	DirectX::XMStoreFloat3(&particleProps.lauchingData.launchPosition, Vector3(0.0f, 0.0f, -10.0f));
	// particleProps.textureID --

	ParticleManager::Get().CreateEffect(particleProps);*/
	
	for (int i = 0; i < 4; i++)
	{
		gameObjects.push_back(new GameObject());

		gameObjects[i]->AddComponent<Rigidbody>();
		gameObjects[i]->GetComponent<Rigidbody>()->SetPosition(Vector4(-16 + (i * 3), 0.0f, -16 + (i * 4), 1.0f));
		gameObjects[i]->GetComponent<Rigidbody>()->SetRotation(Vector4(kYUnitVector), i * 45.0f);
		gameObjects[i]->AddComponent<RenderComponent>();
		gameObjects[i]->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 0.2, 0.2));
		gameObjects[i]->AddComponent<BoxCollider>();
	}

	gameObjects[gameObjects.size() - 1]->AddComponent<StatComponent>();

	SceneGraph::Get()->LoadGraphicsObjects();

	// Nav Mesh
	/*std::vector<Collider2D> obstacles;

	for (auto go : gameObjects)
	{
		go->Update(0.0167f);

		OrientedBoundingBox obb = *go->GetComponent<BoxCollider>()->GetBoundingBox();

		Vector2 center = Vector2(obb.center.GetX(), obb.center.GetZ());
		Vector2 minmin = center - Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) - Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());
		Vector2 maxmin = center + Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) - Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());
		Vector2 minmax = center - Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) + Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());
		Vector2 maxmax = center + Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) + Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());

		Vector3 min = obb.GetMin();
		Vector3 max = obb.GetMax();

		std::vector<Vector2> objectOutline;
		objectOutline.push_back(minmin);
		objectOutline.push_back(maxmin);
		objectOutline.push_back(minmax);
		objectOutline.push_back(maxmax);

		obstacles.push_back(Collider2D(objectOutline));
	}

	Plane groundPlane;
	groundPlane.min = Vector3(-20.0f, 0.0f, -20.0f);
	groundPlane.max = Vector3(20.0f, 0.0f, 20.0f);
	groundPlane.normal = Vector3(kYUnitVector);

	NavMeshManager::GenerateNavMesh(0.25f, groundPlane, obstacles);*/
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
}

void GameScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	ImGui::End();
}
