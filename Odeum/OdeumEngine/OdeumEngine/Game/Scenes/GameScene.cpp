#include "GameScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../../Engine/Math/CollisionHandler.h"
#include "../Components/Rigidbody.h"

GameScene::GameScene() : Scene(), angle(0.0f), direction(1.0f)
{
	object = new GameObject(CubeShape);
	object->AddComponent<Rigidbody>();

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 5.0f, 10.0f));

	CollisionHandler::GetInstance()->Initialize(100.0f);
	CollisionHandler::GetInstance()->AddObject(object);
}

GameScene::~GameScene()
{
	CollisionHandler::GetInstance()->Uninitialize();
}

bool GameScene::Initialize()
{
	LOG("Creating Game Scene");
	return true;
}

void GameScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	angle += direction * (deltaTime_ * 0.1f);

	object->GetComponent<Rigidbody>()->SetRotation(Quaternion(Vector3(kYUnitVector), angle));
	object->Update(deltaTime_);

	Vector4 intersectionPlane;
	Ray mouseRay = CollisionHandler::GetInstance()->GetMouseRay();

	if (Input::Get().isButtonClicked(Button1))
	{
		CollisionHandler::GetInstance()->RayGetFirstHit(mouseRay, &intersectionPlane);
		LOG(intersectionPlane.ToString());
	}
}

void GameScene::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Change rotation direction"))
		direction *= -1.0f;
	ImGui::End();
}
