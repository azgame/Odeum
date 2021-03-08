#include "ComboScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"

ComboScene::ComboScene(){
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 0.5, 1.0));
	gameObjects.back()->SetTag("Floor");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-1.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));
	gameObjects.back()->SetTag("Jump 1");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(3.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));
	gameObjects.back()->SetTag("Jump 2");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(7.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));
	gameObjects.back()->SetTag("Jump 3");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(11.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));
	gameObjects.back()->SetTag("Jump 4");

	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, 0.0f, -6.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5, 1.0, 0.5));
	playerObjects.back()->SetTag("Player 1");


	// push back 4 sets of numbers to the combo (we want multiple uses of 1 key)
	combo.push_back(0);
	combo.push_back(1);
	combo.push_back(2);
	combo.push_back(3);
	combo.push_back(0);
	combo.push_back(1);
	combo.push_back(2);
	combo.push_back(3);
	combo.push_back(0);
	combo.push_back(1);
	combo.push_back(2);
	combo.push_back(3);
	combo.push_back(0);
	combo.push_back(1);
	combo.push_back(2);
	combo.push_back(3);

	std::random_device rd;
	std::default_random_engine rng(rd());

	shuffle(combo.begin(), combo.end(), rng);


	//adding key commands
	playerKeysUp.push_back(Key::W);
	playerKeysLeft.push_back(Key::A);
	playerKeysDown.push_back(Key::S);
	playerKeysRight.push_back(Key::D);
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

	ImGui::Text("Controls:");
	ImGui::Text("Left = 0, right = 1, up = 2, down = 3");


	std::string text = "Combo is: " + std::to_string(combo.at(0)) + " " + std::to_string(combo.at(1)) + " " + std::to_string(combo.at(2)) + " " + std::to_string(combo.at(3));

	const char* textchar = text.c_str();
	ImGui::Text(textchar);


	ImGui::End();
}
