#include "ComboScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"

ComboScene::ComboScene(){
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));
	gameObjects.back()->SetTag("Floor");
}

ComboScene::~ComboScene(){

}

bool ComboScene::Initialize()
{
	return true;
}

void ComboScene::Update(const float deltaTime_){

}

void ComboScene::UIRender(){
	ImGui::Begin("Game UI");

	ImGui::End();
}
