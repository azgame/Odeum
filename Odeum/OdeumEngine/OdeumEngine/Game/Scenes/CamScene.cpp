#include "CamScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"

CamScene::CamScene() : Scene(), angle(0.0f), direction(1.0f)
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 1.5f, -3.0f));
	OdeumEngine::Get().GetCamera().SetLookDirection(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1, 0.0f));
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 0, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1, 1, 1, 1));
	//gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, -1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cow.obj");
	gameObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(1,0.2,0.7,0.5));
}

CamScene::~CamScene()
{
}

bool CamScene::Initialize()
{
	LOG("Creating Game Scene")
		return true;
}

void CamScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	
}

void CamScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	ImGui::End();
}
