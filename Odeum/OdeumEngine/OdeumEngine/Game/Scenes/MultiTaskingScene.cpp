#include "MultiTaskingScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

MultiTaskingScene::MultiTaskingScene()
{
	MaxPlayers = 4;
	timeToStart = 4;
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	for (int i = 0; i < MaxPlayers; i++)
	{
		gameObjectsA.push_back(new GameObject());
		gameObjectsA.back()->AddComponent<Rigidbody>();
		gameObjectsA.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 10-15, -10, 0, 0));
		gameObjectsA.back()->AddComponent<RenderComponent>();
		gameObjectsA.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 0, 1.4));
		playerScores.push_back(0);
		playerPressedA.push_back(false);
	}
	for (int i = 0; i < MaxPlayers; i++)
	{
		gameObjectsB.push_back(new GameObject());
		gameObjectsB.back()->AddComponent<Rigidbody>();
		gameObjectsB.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 10+2.5-15, -10, 0, 0));
		gameObjectsB.back()->AddComponent<RenderComponent>();
		gameObjectsB.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 1.4, 0));
		playerPressedB.push_back(false);
	}
	for (int i = 0; i < MaxPlayers; i++)
	{
		gameObjectsC.push_back(new GameObject());
		gameObjectsC.back()->AddComponent<Rigidbody>();
		gameObjectsC.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 10+5-15, -10, 0, 0));
		gameObjectsC.back()->AddComponent<RenderComponent>();
		gameObjectsC.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.4, 0, 0));
		playerPressedC.push_back(false);
	}
	playerKeysA.push_back(Key::H);
	playerKeysA.push_back(Key::H);
	playerKeysA.push_back(Key::H);
	playerKeysA.push_back(Key::H);
	playerKeysB.push_back(Key::J);
	playerKeysB.push_back(Key::J);
	playerKeysB.push_back(Key::J);
	playerKeysB.push_back(Key::J);
	playerKeysC.push_back(Key::K);
	playerKeysC.push_back(Key::K);
	playerKeysC.push_back(Key::K);
	playerKeysC.push_back(Key::K);
	targetHeights.push_back(-4);
	targetHeights.push_back(2);
	targetHeights.push_back(8);
	targetObjects.push_back(new GameObject());
	targetObjects.back()->AddComponent<Rigidbody>();
	targetObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-17.5, -4, 0, 0));
	targetObjects.back()->AddComponent<RenderComponent>();
	targetObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.2, 0, 1.4));
	targetObjects.push_back(new GameObject());
	targetObjects.back()->AddComponent<Rigidbody>();
	targetObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-17.5, 2, 0, 0));
	targetObjects.back()->AddComponent<RenderComponent>();
	targetObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.2, 0, 1.4));
	targetObjects.push_back(new GameObject());
	targetObjects.back()->AddComponent<Rigidbody>();
	targetObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-17.5, 8, 0, 0));
	targetObjects.back()->AddComponent<RenderComponent>();
	targetObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.2, 0, 1.4));
	//playerKeys.push_back(Key::L);
}

MultiTaskingScene::~MultiTaskingScene()
{
}

bool MultiTaskingScene::Initialize()
{
	return true;
}

void MultiTaskingScene::Update(const float deltaTime_)
{
	timeToStart -= deltaTime_;
	UpdateObjectsB( deltaTime_);
	UpdateObjectsC( deltaTime_);
	//gameObjects.back()->GetComponent<Rigidbody>()->
	//gameObjects.back()->GetComponent<Rigidbody>()->ApplyForce(Vector4(0, -0.05, 0, 0));
	if (!won)
	{
		if (timeToStart <= 0)
			for (int i = 0; i < gameObjectsA.size(); i++)
			{
				if (gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > -10.50f)
					gameObjectsA.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.006, 0, 0));
				else
					gameObjectsA.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 1, 0, 0));
				
				
				if (Input::Get().isKeyPressed(playerKeysA.at(i)))
				{
					if (!playerPressedA.at(i))
						gameObjectsA.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 1.0f, 0, 0));
					playerPressedA.at(i) = true;

				}
				else
				{
					playerPressedA.at(i) = false;
				}
				if (gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 10.50f)
					gameObjectsA.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, -1, 0, 0));
				if (gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > targetHeights.at(0)-1.5f&& gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < targetHeights.at(0) + 2.5f)
				{
					playerScores.at(i) += deltaTime_;
				}


			}
		
	}
	else
	{
		for (int i = 0; i < gameObjectsA.size(); i++)
		{
			gameObjectsA.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
		}
	}




}
void MultiTaskingScene::UpdateObjectsB(const float deltaTime_)
{
	if (!won)
	{
		if (timeToStart <= 0)
			for (int i = 0; i < gameObjectsB.size(); i++)
			{
				if (gameObjectsB.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > -10.50f)
					gameObjectsB.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.004, 0, 0));
				else
					gameObjectsB.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 1.25f, 0, 0));


				if (Input::Get().isKeyPressed(playerKeysB.at(i)))
				{
					if (!playerPressedB.at(i))
						gameObjectsB.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 1.25f, 0, 0));
					playerPressedB.at(i) = true;

				}
				else
				{
					playerPressedB.at(i) = false;
				}
				if (gameObjectsB.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 10.50f)
					gameObjectsB.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, -1, 0, 0));
				if (gameObjectsB.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > targetHeights.at(1) - 1.5f && gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < targetHeights.at(1) + 2.5f)
				{
					playerScores.at(i) += deltaTime_;
				}
			}

	}
	else
	{
		for (int i = 0; i < gameObjectsB.size(); i++)
		{
			gameObjectsB.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
		}
	}
}
void MultiTaskingScene::UpdateObjectsC(const float deltaTime_)
{
	if (!won)
	{
		if (timeToStart <= 0)
			for (int i = 0; i < gameObjectsC.size(); i++)
			{
				if (gameObjectsC.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > -10.50f)
					gameObjectsC.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.004, 0, 0));
				else
					gameObjectsC.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 1.75f, 0, 0));


				if (Input::Get().isKeyPressed(playerKeysC.at(i)))
				{
					if (!playerPressedC.at(i))
						gameObjectsC.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 1.5f, 0, 0));
					playerPressedC.at(i) = true;

				}
				else
				{
					playerPressedC.at(i) = false;
				}
				if (gameObjectsC.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 10.50f)
					gameObjectsC.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, -1, 0, 0));
				if (gameObjectsC.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > targetHeights.at(2) - 1.5f && gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < targetHeights.at(2) + 2.5f)
				{
					playerScores.at(i) += deltaTime_;
				}
			}

	}
	else
	{
		for (int i = 0; i < gameObjectsC.size(); i++)
		{
			gameObjectsC.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
		}
	}
}
void MultiTaskingScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	for (int i = 0; i < gameObjectsA.size(); i++)
	{
		if (gameObjectsA.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 15.0f)
		{
			std::string text = "Player" + std::to_string(i + 1) + " Wins";
			const char* textchar = text.c_str();
			won = true;
			ImGui::Text(textchar);

		}
		std::string text = "Player Score" + std::to_string(playerScores.at(i));
		const char* textchar = text.c_str();
		ImGui::Text(textchar);

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
