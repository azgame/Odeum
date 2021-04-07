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
	go1->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5f, 1.0f, 1.0f, 1.0f));
	//go1->GetComponent<RenderComponent>()->LoadModelFromFile("Pig.obj");

	go1->AddComponent<Rigidbody>();
	go1->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 5.0f, 0.0f, 0.0f));

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
}

void AnimationTestScene::UIRender()
{

}
