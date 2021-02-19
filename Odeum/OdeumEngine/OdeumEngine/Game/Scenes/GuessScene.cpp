#include "GuessScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include <random>
#include <algorithm>

using std::shuffle;

GuessScene::GuessScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	// Door1
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2.0f, 5.0f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5, 0.5, 0.5));

	//Door2
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2.0f, 5.0f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5, 0.5, 0.5));

	//Door3
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2.0f, 5.0f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5, 0.5, 0.5));

	//Floor
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, -0.9f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));

	//reward
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 7.0f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 4.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 0.0, 1.0));

	// p1 (0)
	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, 0.0f, -4.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.5f, 1.0f));



	std::vector<int> i_vec1;
	i_vec1.push_back(0);
	i_vec1.push_back(1);
	i_vec1.push_back(2);

	std::random_device rd;
	std::default_random_engine rng(rd());

	shuffle(i_vec1.begin(), i_vec1.end(), rng);

	gameObjects.at(4)->GetComponent<Rigidbody>()->SetPosition(gameObjects.at(i_vec1.front())->GetComponent<Rigidbody>()->GetPosition());


}


GuessScene::~GuessScene() {
};

bool GuessScene::Initialize()
{
	return true;
}

void GuessScene::Update(const float deltaTime_) {
	
	if (Input::Get().isKeyPressed(Key::A)) {
		if (playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetX() == -5.0f) {

		} else {
			playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.back()->GetComponent<Rigidbody>
				()->GetPosition().GetX() - 5.0f, 0.0f, -4.0f, 0.0f) );
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

	if (Input::Get().isKeyPressed(Key::E)) {
		for (int i = 0; i < 3; i++)
		{
			if (gameObjects.at(i)->GetComponent<Rigidbody>
				()->GetPosition().GetX() == playerObjects.back()->GetPosition().GetX()){
			
				gameObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4())

			}
		}
	}

}



void GuessScene::UIRender()
{ 
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");



	ImGui::End();
}