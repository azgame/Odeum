#include "BenScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/Rigidbody.h"
#include "../../Engine/Math/CollisionHandler.h"
#include "../Engine/Core/AudioHandler.h"
#include "../Components/AudioSource.h"


BenScene::BenScene() : Scene(), angle(0.0f), direction(1.0f)
{
	// should pass in the camera pos
	AudioHandler::GetInstance()->Initialize();
	
	object = new GameObject(CubeShape, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	object2 = new GameObject(CubeShape, Colour(0.1f, 0.3f, 0.4f, 1.0f));

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, 25.0f));

	//Physics Test
	object->AddComponent<Rigidbody>();
	object->GetComponent<Rigidbody>()->SetPosition(Vector4(10.0f, 0.0f, 0.0f, 0.0f));
	object->GetComponent<Rigidbody>()->AddVelocity(Vector4(-1.0f, 0.0f, 0.0f, 0.0f));
	object->GetComponent<Rigidbody>()->AddAngularVelocity(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 0.002f);

	object2->AddComponent<Rigidbody>();
	object2->GetComponent<Rigidbody>()->SetPosition(Vector4(-10.0f, 0.0f, 0.0f, 0.0f));
	object2->GetComponent<Rigidbody>()->AddVelocity(Vector4(1.0f, 0.0f, 0.0f, 0.0f));
	object2->GetComponent<Rigidbody>()->AddAngularVelocity(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 0.002f);

	// AudioTest
	object->AddComponent<AudioSource>();
	object->GetComponent<AudioSource>()->Initialize("TestCoin.wav", false, true, false, 1.0f);
	object->GetComponent<AudioSource>()->PlaySound();

	CollisionHandler::GetInstance()->Initialize(1000.0f);

	CollisionHandler::GetInstance()->AddObject(object);
	CollisionHandler::GetInstance()->AddObject(object2);
}

BenScene::~BenScene()
{
	AudioHandler::GetInstance()->OnDestroy();
}

bool BenScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);
	return true;
}

void BenScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	object->Update(deltaTime_);
	object2->Update(deltaTime_);

	CollisionHandler::GetInstance()->Update();
	CollisionHandler::GetInstance()->MouseUpdate();

	if (Input::Get().isKeyPressed(Key::KeyCode::A)) {
		Debug::Info("PLAY SOUND!", "BenScene.cpp", __LINE__);
		object->GetComponent<AudioSource>()->PlaySound();
	}
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