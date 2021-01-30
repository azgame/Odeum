#include "ButtonMashingScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"

ButtonMashingScene::ButtonMashingScene()
{
	MaxPlayers = 4;
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	for (int i = 0; i < MaxPlayers; i++)
	{
		gameObjects.push_back(new GameObject(ShapeTypes::CubeShape, Colour(i, i, i)));
		gameObjects.back()->AddComponent<Rigidbody>();
		gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i, -10, 0, 0));
		playerPressed.push_back(false);
	}
	
	

	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
	SceneGraph::Get()->LoadObjectsIntoMemory();
	SceneGraph::Get()->UpdateObjects(0.0167f);
}

ButtonMashingScene::~ButtonMashingScene()
{
}

bool ButtonMashingScene::Initialize()
{
	return false;
}

void ButtonMashingScene::Update(const float deltaTime_)
{
	SceneGraph::Get()->UpdateObjects(deltaTime_);
	//gameObjects.back()->GetComponent<Rigidbody>()->
	//gameObjects.back()->GetComponent<Rigidbody>()->ApplyForce(Vector4(0, -0.05, 0, 0));
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.005, 0, 0));
		if (Input::Get().isKeyPressed(playerKeys.at(i)))
		{
			if (!playerPressed.at(i))
				gameObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 1.5f, 0, 0));
			playerPressed.at(i) = true;
			
		}
		else
		{
			playerPressed.at(i) = false;
		}

	}

	
	

	//OdeumEngine::Get().GetCamera().SetPosition(Vector3(gameObjects.back()->GetComponent<Rigidbody>()->GetPosition()) + Vector3(0.0f, 10.0f, -25.0f));
	/*OdeumEngine::Get().GetCamera().SetLookDirection( 
		Vector3(gameObjects.back()->GetComponent<Rigidbody>()->GetPosition()), 
		Vector3(kYUnitVector)
	);*/
}

void ButtonMashingScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 25.0f)
		{
			std::string text = "Player "+i;
			const char* textchar = text.c_str();
			
			ImGui::Text(textchar);
		}
		
	}
	

	ImGui::End();
}
