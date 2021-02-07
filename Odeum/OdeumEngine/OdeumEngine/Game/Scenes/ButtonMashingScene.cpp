#include "ButtonMashingScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

ButtonMashingScene::ButtonMashingScene()
{
	MaxPlayers = 4;
	timeToStart = 4;
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	for (int i = 0; i < MaxPlayers; i++)
	{
		gameObjects.push_back(new GameObject());
		gameObjects.back()->AddComponent<Rigidbody>();
		gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i*5, -10, 0, 0));
		gameObjects.back()->AddComponent<RenderComponent>();
		gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(i, i, i));
		playerPressed.push_back(false);
	}
	
	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
}

ButtonMashingScene::~ButtonMashingScene()
{
}

bool ButtonMashingScene::Initialize()
{
	return true;
}

void ButtonMashingScene::Update(const float deltaTime_)
{
	timeToStart -= deltaTime_;
	//gameObjects.back()->GetComponent<Rigidbody>()->
	//gameObjects.back()->GetComponent<Rigidbody>()->ApplyForce(Vector4(0, -0.05, 0, 0));
	if (!won)
	{
		if (timeToStart <= 0)
			for (int i = 0; i < gameObjects.size(); i++)
			{
				if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > -10.50f)
					gameObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.002, 0, 0));
				else
					gameObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
				if (Input::Get().isKeyPressed(playerKeys.at(i)))
				{
					if (!playerPressed.at(i))
						gameObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0.5f, 0, 0));
					playerPressed.at(i) = true;

				}
				else
				{
					playerPressed.at(i) = false;
				}

			}
	}
	else
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
		}
	}
	
	

	
}

void ButtonMashingScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 15.0f)
		{
			std::string text  = "Player"+std::to_string(i+1)+" Wins";
			const char* textchar = text.c_str();
			won = true;
			ImGui::Text(textchar);
		}
		
	}
	if (timeToStart > 0)
	{
		ImGui::Text("wait for it");
		
		std::string timeText = std::to_string(ceil(timeToStart));
		const char* timechar = timeText.c_str();
		
		ImGui::Text(timechar);
	}
	else
	{ 
		ImGui::Text("MASH!");
	}

	ImGui::End();
}
