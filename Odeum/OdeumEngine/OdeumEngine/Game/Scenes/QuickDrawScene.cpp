#include "QuickDrawScene.h"

#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include "../../Engine/CoreComponents/AudioSource.h"
#include"../../Engine/Core/AudioHandler.h"
QuickDrawScene::QuickDrawScene()
{
	AudioHandler::GetInstance()->Initialize();
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 30, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 10));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cake.obj");
	gameObjects.back()->AddComponent<AudioSource>();
	gameObjects.back()->GetComponent<AudioSource>()->Initialize("CanCan.mp3", true, false, false, 1.0f);
	//gameObjects.back()->GetComponent<AudioSource>()->PlaySound();
	
	maxPlayers = 4;
	maxScore = 3;
	for (int i = 0; i < maxPlayers; i++)
	{
		players.push_back(new GameObject());
		players.back()->AddComponent<Rigidbody>();
		players.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i*4-6, 3, -15, 0));
		players.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 10));
		players.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, -1, 0, 0),90);
		players.back()->AddComponent<RenderComponent>();
		players.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cakeSlicer.obj");
		playerScore.push_back(0);
		playerShootTimer.push_back(0);
		playerInPosition.push_back(false);
	}
	players.back()->AddComponent<AudioSource>();
	players.back()->GetComponent<AudioSource>()->Initialize("TestCoin.wav", false, false, false, 1.0f);
	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
	srand(time(NULL));
	timeUntilTarget = rand() % 7 + 1.5f;
}

QuickDrawScene::~QuickDrawScene()
{
}

bool QuickDrawScene::Initialize()
{
	return true;
}

void QuickDrawScene::Update(const float deltaTime_)
{
	if (!won)
	{
		if (timeToStart <= 0)
		{
			if (timeUntilTarget <= 0)
			{
				gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(objectPos);
			}
			else {
				timeUntilTarget -= deltaTime_;
				gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 30, 0, 0));
			}
			for (int i = 0; i < players.size(); i++)
			{
				if (playerShootTimer.at(i) > 0)
				{
					playerShootTimer.at(i) -= deltaTime_;
				}
				else if (playerInPosition.at(i) == false)
				{
					players.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 4 - 6, 3, -15, 0));
					playerInPosition.at(i) = true;

				}

				if (Input::Get().isKeyPressed(playerKeys.at(i)))
				{

					if (playerShootTimer.at(i) <= 0)
					{

						playerInPosition.at(i) = false;
						if (timeUntilTarget <= 0)
						{
							players.at(i)->GetComponent<Rigidbody>()->SetPosition(slicerPos + Vector4(i * 1.3f - 2, 0, 0, 0));
							targetHit = true;
							playerScore.at(i) += 1;
							//players.back()->GetComponent<AudioSource>()->PlaySound();
							if (playerScore.at(i) == maxScore)
							{
								won = true;
								
							}
						}
						else
						{
							players.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 4 - 6, 3, -15, 0) + Vector4(0, 2, 2, 0));
							playerShootTimer.at(i) = 2;
						}
					}

				}
			}

			if (targetHit)
			{
				if (!won)
				{
					srand(time(NULL));
					timeUntilTarget = rand() % 7 + 1.5f;
					timeToStart = 1.0f;
					targetHit = false;
					
				}

			}
		}
		else
		{
			timeToStart -= deltaTime_;
		}
	}
	
}

void QuickDrawScene::UIRender()
{

	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	
	for (int i = 0; i < players.size(); i++)
	{
		std::string text = "Player" + std::to_string(i + 1) +"Score: " + std::to_string(playerScore.at(i));
		const char* textchar = text.c_str();
		ImGui::Text(textchar);
		 text = "Player" + std::to_string(i + 1) + "Counter: " + std::to_string(playerShootTimer.at(i));
		 textchar = text.c_str();
		ImGui::Text(textchar);
		if (playerScore.at(i) == maxScore)
		{
			text = "Player" + std::to_string(i + 1) + "Wins ";
			textchar = text.c_str();
			ImGui::Text(textchar);
		}
	}
	std::string text = "time until target: " + std::to_string(timeUntilTarget);
		const char* textchar = text.c_str();
		ImGui::Text(textchar);
	ImGui::End();
	
	
}
