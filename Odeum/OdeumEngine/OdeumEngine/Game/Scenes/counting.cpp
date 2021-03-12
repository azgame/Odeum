#include "Counting.h"

#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
Counting::Counting()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
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
			gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(rand() % 3, rand() % 3, rand() % 3));
			//gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(50, 250, 0.0));
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
}

void Counting::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
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
	}
	std::string text = "MaxCount" + std::to_string(maxCount);
	const char* textchar = text.c_str();
	ImGui::Text(textchar);
	text = "count" + std::to_string(count);
	 textchar = text.c_str();
	ImGui::Text(textchar);
	text = "time to win" + std::to_string(timeToWin);
	textchar = text.c_str();
	ImGui::Text(textchar);
	ImGui::End();
}
