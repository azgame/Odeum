#include "Map.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"

#include <algorithm>



MAPScene::MAPScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	/*
	// NODE1
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2.0f, 5.0f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5, 0.5, 0.5));

	*/
	

}


MAPScene::~MAPScene() {
};

bool MAPScene::Initialize()
{
	return true;
}

void MAPScene::Update(const float deltaTime_) {
	/*
	if (Input::Get().isKeyPressed(Key::A)) {
		if (playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetX() == -5.0f) {

		}
		else {
			playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.back()->GetComponent<Rigidbody>
				()->GetPosition().GetX() - 5.0f, 0.0f, -4.0f, 0.0f));
		}
	}
	if (Input::Get().isKeyPressed(Key::D)) {
		if (playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetX() == 5.0f) {

		}
		else {
			playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.back()->GetComponent<Rigidbody>
				()->GetPosition().GetX() + 5.0f, 0.0f, -4.0f, 0.0f));
		}
	}
	*/ //later input stuff

	//later inputstuff
}



void MAPScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");



	ImGui::End();
}