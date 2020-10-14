#include "BenScene.h"
#include "../Components/KinimaticMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/ComponentTest.h"
#include "../Components/Rigidbody.h"
#include "../../Engine/Math/CollisionHandler.h"

BenScene::BenScene() : Scene(), angle(0.0f), direction(1.0f)
{
	object = new GameObject(CubeShape, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	object2 = new GameObject(CubeShape, Colour(0.1f, 0.3f, 0.4f, 1.0f));

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, 25.0f));

	object->AddComponent<Rigidbody>();
	object->GetComponent<Rigidbody>()->SetPosition(Vector4(10.0f, 0.0f, 0.0f, 0.0f));
	object->GetComponent<Rigidbody>()->AddVelocity(Vector4(-1.0f, 0.0f, 0.0f, 0.0f));
	object->GetComponent<Rigidbody>()->AddAngularVelocity(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 0.002f);

	object2->AddComponent<Rigidbody>();
	object2->GetComponent<Rigidbody>()->SetPosition(Vector4(-10.0f, 0.0f, 0.0f, 0.0f));
	object2->GetComponent<Rigidbody>()->AddVelocity(Vector4(1.0f, 0.0f, 0.0f, 0.0f));
	object2->GetComponent<Rigidbody>()->AddAngularVelocity(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 0.002f);
}

BenScene::~BenScene()
{
}

bool BenScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);
	return true;
}

void BenScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	CollisionHandler::GetInstance()->MouseUpdate();
	object->Update(deltaTime_);
	object2->Update(deltaTime_);
}

void BenScene::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Reset Position"))
	{
		object->GetComponent<Rigidbody>()->SetPosition(Vector4(10.0f, 0.0f, 0.0f, 0.0f));
		object2->GetComponent<Rigidbody>()->SetPosition(Vector4(-10.0f, 0.0f, 0.0f, 0.0f));
	}
	ImGui::End();
}