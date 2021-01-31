#include "Counting.h"

#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

Counting::Counting()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	MaxPlayers = 4;
	maxCount = 15;
	for (int i = 0; i < MaxPlayers; i++)
	{
		players.push_back(new GameObject());
		playerCount.push_back(0);
		playerPressed.push_back(false);
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
	return true;
}

void Counting::Update(const float deltaTime_)
{
	if (timeToWin > 0)
	{
		timeToWin - deltaTime_;
	}
	else
	{
		won = true;
	}
	if (!won)
	{
		timeToSpawn -= deltaTime_;
		if (timeToSpawn <= 0&&maxCount>0)
		{
			timeToSpawn = 0.5f;
			maxCount -= 1;
			gameObjects.push_back(new GameObject());
			gameObjects.back()->AddComponent<Rigidbody>();
			gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-1, 0, 0, 0));
			gameObjects.back()->GetComponent<Rigidbody>()->SetVelocity(Vector4(2, 0, 0, 0));
			gameObjects.back()->AddComponent<RenderComponent>();
			gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(2.2f, 0.5f, 0.5f));
			if (maxCount <= 0)
			{
				timeToWin = 5.0f;
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
	ImGui::End();
}
