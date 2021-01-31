#include "QuickDrawScene.h"

#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

QuickDrawScene::QuickDrawScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	maxPlayers = 4;
	for (int i = 0; i < maxPlayers; i++)
	{
		players.push_back(new GameObject());
		playerScore.push_back(0);
		playerShootTimer.push_back(0);
	}
	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
	timeUntilTarget = 5;
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
	timeUntilTarget -= deltaTime_;
	for (int i = 0; i < players.size(); i++)
	{
		playerShootTimer.at(i) -= deltaTime_;
		if (Input::Get().isKeyPressed(playerKeys.at(i)))
		{
			
			if (playerShootTimer.at(i) <= 0)
			{
				if (timeUntilTarget <= 0)
				{
					targetHit = true;
					playerScore.at(i) += 1;
				}
				else
				{
					playerShootTimer.at(i) = 2;
				}
			}
			
		}
	}
	if (targetHit)
	{
		timeUntilTarget = 5;
		targetHit = false;
		
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
	}
	std::string text = "time until target: " + std::to_string(timeUntilTarget);
		const char* textchar = text.c_str();
		ImGui::Text(textchar);
	ImGui::End();
	
	
}
