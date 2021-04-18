#include "Counting.h"

#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include"../../Engine/Core/AudioHandler.h"
#include "../../Engine/CoreComponents/AudioSource.h"
Counting::Counting()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	OdeumEngine::Get().GetCamera().SetLookDirection(Vector3(0.0, 0.0, 0.0));
	MaxPlayers = 4;
	
	srand(time(NULL));
	maxCount = rand() % 20 + 20;
	count = maxCount;
	for (int i = 0; i < MaxPlayers; i++)
	{
		players.push_back(new GameObject());
		playerCount.push_back(0);
		playerPressed.push_back(false);
		playerWon.push_back(false);
		playerScore.push_back(false);
	}
	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
	timeToWin = 999;

	players.back()->AddComponent<AudioSource>();
	players.back()->GetComponent<AudioSource>()->Initialize("CanCan.mp3", true, false, false, 0.5f);
	players.back()->GetComponent<AudioSource>()->PlaySound();

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, -3, 0, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(100, 2, 100, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cliff_block_rock.obj");
	backgroundObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(0, 0, 0, 0.8));
	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 19, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/log_stack.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-8, -1.1, -8, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(8, 8, 8, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(12, -1.1, -5, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(8, 8, 8, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(7.2, -1.1, 23, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(12, 12, 12, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-15, -1.1, 28, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(6, 6, 6, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(14, -1.1, 22, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(7, 7, 7, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(14, -1.1, 44, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(13, 13, 13, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-33, -1.1, 44, 0));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(17, 17, 17, 1));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

}

Counting::~Counting()
{
}

bool Counting::Initialize()
{
	LOG("Creating Counting Scene")
	return true;
}
void Counting::CalculateWinner()
{
	bool stop=false;
	for (int i = 0; i < players.size(); i++)
	{
		playerScore.at(i) = playerCount.at(i) - maxCount;
		if (playerScore.at(i) > 0)
		{
			playerScore.at(i) = -maxCount;
		}
	}
	for (int z = 0; z > -maxCount-1; z--)
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (playerScore.at(i) == z)
			{
				playerWon.at(i) = true;
				stop = true;
			}
		}
		if (stop)
			return;

	}
}
void Counting::Update(const float deltaTime_)
{
	if (timeToWin > 0)
	{
		timeToWin -= deltaTime_;
	}
	else
	{
		if (won == false)
		{
			CalculateWinner();
		}
		won = true;
	}
	if (!won)
	{
		timeToSpawn -= deltaTime_;
		if (timeToSpawn <= 0&&count>0)
		{
			
			
			timeToSpawn = rand() % 1+0.4f;
			count -= 1;
			gameObjects.push_back(new GameObject());
			gameObjects.back()->AddComponent<Rigidbody>();
			gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-30, 0, rand() % 14 , 0));
			gameObjects.back()->GetComponent<Rigidbody>()->SetVelocity(Vector4(rand() % 16 + 2.2f, 0, 0, 0));
			gameObjects.back()->AddComponent<RenderComponent>();
			//gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(rand() % 3, rand() % 3, rand() % 3));
			gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Sheep.obj");
			gameObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(rand() % 2, rand() % 2, rand() % 2,0.2));
			//gameObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(1, 0, 0,0.2));
			//gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 10));
			gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 90);
			if (count == 0)
			{
				timeToWin = 10.0f;
			}
		}
		for (int i = 0; i < players.size(); i++)
		{
			if (Input::Get().isKeyPressed(playerKeys.at(i)))
			{
				if (!playerPressed.at(i))
				{
					playerCount.at(i)++;
				}
				playerPressed.at(i) = true;

			}
			else
			{
				playerPressed.at(i) = false;
			}
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

void Counting::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Count the Sheep without going over");
	for (int i = 0; i < players.size(); i++)
	{
		std::string text = "Player" + std::to_string(i + 1) + "Count: " + std::to_string(playerCount.at(i));
		const char* textchar = text.c_str();
		ImGui::Text(textchar);
		
	}
	if (won)
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (playerWon.at(i) == true)
			{
				std::string text = "Player" + std::to_string(i + 1) + "Wins ";
				const char* textchar = text.c_str();
				ImGui::Text(textchar);
			}

		}
		std::string text = "correct number " + std::to_string(maxCount);
		const char* textchar = text.c_str();
		ImGui::Text(textchar);
	}
	else
	{
		if (timeToWin < 10)
		{
			std::string text = "time left " + std::to_string(timeToWin);
			const char* textchar = text.c_str();
			ImGui::Text(textchar);
		}
	}
	ImGui::End();
}
