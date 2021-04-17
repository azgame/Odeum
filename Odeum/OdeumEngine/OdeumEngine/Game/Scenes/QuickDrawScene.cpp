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
	playerColors.push_back(&Colour(0, 0, 1, 0.5));
	playerColors.push_back(&Colour(1, 0, 0, 0.5));
	playerColors.push_back(&Colour(1, 1, 0, 0.5));
	playerColors.push_back(&Colour(0, 1, 0, 0.5));
	AudioHandler::GetInstance()->Initialize();
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0,-4.5,0,0)+objectPos);
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 10));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/stall.obj");
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, -5, 0, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0),90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0),180.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Chair_1.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5, -5, -6.5, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Chair_1.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(5, -5, -6.5, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, -1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Chair_1.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15, -5, 10, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, -1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Kitchen_Fridge.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-17, -5, 50, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 180.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Couch_Large2.fbx");
	gameObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(1, 1, 1, 0.1));
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, -5, 50, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Column_Round2.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 3, 30, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Houseplant_2.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-35, -5, 50, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Light_Stand1.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15, -5, 20, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, -1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Kitchen_Oven_Large.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15, -5, 0, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5, 5, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90.0f);
	gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, -1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Trashcan_Large.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-45, -5, 30, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 2, 2, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 90.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cabinWall.fbx");

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, -5, 60, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 2, 2, 1));
	//gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 90.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cabinWall.fbx");
	gameObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(1, 1, 1, 0.1));

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20, -5, 15, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 2, 2, 1));
	gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 90.0f);
	//gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90.0f);
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cabinWall.fbx");
	
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, -1, 500, 0));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1, 1, 1, 1));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cakeBirthday.fbx");
	gameObjects.back()->AddComponent<AudioSource>();
	gameObjects.back()->GetComponent<AudioSource>()->Initialize("CanCan.mp3", true, false, false, 0.5f);
	gameObjects.back()->GetComponent<AudioSource>()->PlaySound();
	
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

		players.back()->GetComponent<RenderComponent>()->SetColour(*playerColors.at(i));
	}
	players.back()->AddComponent<AudioSource>();
	players.back()->GetComponent<AudioSource>()->Initialize("ding.wav", false, false, false, 1.0f);
	
	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
	srand(time(NULL));
	timeUntilTarget = rand() % 7 + 1.5f;
}

QuickDrawScene::~QuickDrawScene()
{
	for (int i = 0; i < playerColors.size(); i++)
	{
		delete (playerColors[i]);
	}
	playerColors.clear();
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
				gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(3, 0, 0, 0)+objectPos);
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
							players.back()->GetComponent<AudioSource>()->PlaySound();
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
	if (won)
	{
		if (timeToEnd >= 0)
		{
			timeToEnd -= deltaTime_;
			if (timeToEnd <= 0)
			{
				OdeumEngine::Get().SetCurrentScene(19);
				//return who wins
			}
		}
	}
}

void QuickDrawScene::UIRender()
{

	ImGui::Begin("Game UI");

	ImGui::Text("Grab the cake first");
	for (int i = 0; i < players.size(); i++)
	{
		if (!won)
		{
			std::string text = "Player" + std::to_string(i + 1) + "Score: " + std::to_string(playerScore.at(i));
			const char* textchar = text.c_str();
			ImGui::Text(textchar);
		}
		else
		{
			if (playerScore.at(i) == maxScore)
			{
				std::string text = "Player" + std::to_string(i + 1) + "Wins ";
				const char* textchar = text.c_str();
				ImGui::Text(textchar);
			}
		}
	}
	
	ImGui::End();
	
	
}
