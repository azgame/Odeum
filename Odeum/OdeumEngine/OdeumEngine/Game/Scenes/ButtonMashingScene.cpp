#include "ButtonMashingScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../../Engine/CoreComponents/AudioSource.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
ButtonMashingScene::ButtonMashingScene()
{
	MaxPlayers = 4;
	timeToStart = 4;
	AudioHandler::GetInstance()->Initialize();
	srand(time(NULL));
	playerColors.push_back(&Colour(0,0,1,0.5));
	playerColors.push_back(&Colour(1,0,0,0.5));
	playerColors.push_back(&Colour(1,1,0,0.5));
	playerColors.push_back(&Colour(0,1,0,0.5));
	for (int i = 0; i < MaxPlayers; i++)
	{
		moonObjects.push_back(new GameObject());
		
		moonObjects.back()->AddComponent<Rigidbody>();
		moonObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 8 - 10, 30, 0, 0));
		moonObjects.back()->AddComponent<RenderComponent>();
		moonObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Planet1.obj");
		moonObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
	}
	
	moonObjects.back()->AddComponent<AudioSource>();
	moonObjects.back()->GetComponent<AudioSource>()->Initialize("CanCan.mp3", true, false, false, 0.5f);
	moonObjects.back()->GetComponent<AudioSource>()->PlaySound();
	
	
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -55.0f));
	OdeumEngine::Get().GetCamera().SetRotation(Quaternion());
	for (int i = 0; i < MaxPlayers; i++)
	{
		gameObjects.push_back(new GameObject());
		gameObjects.back()->AddComponent<Rigidbody>();
		gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i*8-10, -10, 0, 0));
		gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 90);
		
		gameObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 0, -1, 0), 90);
		gameObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90);
		gameObjects.back()->AddComponent<RenderComponent>();
		//gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(i, i, i));
		gameObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/SpaceShip.obj");
		playerPressed.push_back(false);
		gameObjects.back()->GetComponent<RenderComponent>()->SetColour(*playerColors.at(i));
	}
	for (int i = 0; i < 75; i++)
	{
		backgroundObjects.push_back(new GameObject());
		backgroundObjects.back()->AddComponent<Rigidbody>();
		backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(rand() % 100-30, i+(rand() % 5)-15, 10, 0));
		backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(rand() % 2+1,  (rand() % 2) +1, (rand() % 2) + 1, 1));

		backgroundObjects.back()->AddComponent<RenderComponent>();
		backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Asteroid1.obj");

	}


	//_______ particles __________
	rocketProps.startColour = DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
	rocketProps.endColour = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	rocketProps.lifeTime = FLT_MAX;

	rocketProps.minLife = 1.0f;
	rocketProps.maxLife = 2.5f;

	rocketProps.minMass = 0.1f;
	rocketProps.maxMass = 0.4f;

	rocketProps.rotationMax = 0.0f;
	rocketProps.spread = Vector3(0.0f, 100.0f, 0.0f);

	// speed
	rocketProps.minVelocity = Vector2(1.f, 1.f);
	rocketProps.maxVelocity = Vector2(10.0f, 10.0f);


	rocketProps.minSize = Vector2(0.1f, 0.1f);
	rocketProps.maxSize = Vector2(0.4f, 0.4f);

	// yAxis set to -1 to reverse the particles
	DirectX::XMStoreFloat3(&rocketProps.lauchingData.xAxis, Vector3(kXUnitVector));
	DirectX::XMStoreFloat3(&rocketProps.lauchingData.yAxis, Vector3(0.0f, -1.0f, 0.0f));
	DirectX::XMStoreFloat3(&rocketProps.lauchingData.zAxis, Vector3(kZUnitVector));

	rocketProps.lauchingData.maxParticles = 300;
	rocketProps.lauchingData.spawnRate = 64.0f;
	rocketProps.lauchingData.speed = 1.0f;

	// dont want the particles to bounce
	rocketProps.lauchingData.groundBounce = 0.0f;

	DirectX::XMStoreFloat3(&rocketProps.lauchingData.gravity, Vector3(0.0f, -9.8f, 0.0f));
	DirectX::XMStoreFloat3(&rocketProps.lauchingData.launchPosition, Vector3(0.0f, 0.0f, 0.0f));
	// particleProps.textureID --

	//pEffect = &ParticleManager::Get().CreateEffect(rocketProps);
	
	playerKeys.push_back(Key::H);
	playerKeys.push_back(Key::J);
	playerKeys.push_back(Key::K);
	playerKeys.push_back(Key::L);
	
}

ButtonMashingScene::~ButtonMashingScene()
{
	for (int i = 0; i < playerColors.size(); i++)
	{
		delete (playerColors[i]);
	}
	playerColors.clear();
}

bool ButtonMashingScene::Initialize()
{
	return true;
}

void ButtonMashingScene::Update(const float deltaTime_)
{
	
	//cameraController.UpdateMainCamera();
	cameraController.UpdateMainCamera();
	timeToStart -= deltaTime_;
	//gameObjects.back()->GetComponent<Rigidbody>()->
	//gameObjects.back()->GetComponent<Rigidbody>()->ApplyForce(Vector4(0, -0.05, 0, 0));
	if (!won)
	{
		if (timeToStart <= 0)
			for (int i = 0; i < gameObjects.size(); i++)
			{
				if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > -10.50f)
					gameObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -deltaTime_*0.35f, 0, 0));
				else
					gameObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
				if (Input::Get().isKeyPressed(playerKeys.at(i)))
				{
					if (!playerPressed.at(i))
					{
						if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() <= -5)
						{
							gameObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
						}
						gameObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 55 * deltaTime_, 0, 0));



						//particles for the rocketship
						//rocketProps.maxVelocity = Vector2(-55.0f * deltaTime_, -55.0f * deltaTime_);
						pEffect = &ParticleManager::Get().CreateEffect(rocketProps);
						pEffect->Translate(gameObjects.at(i)->GetPosition());
					}
					playerPressed.at(i) = true;

				}
				else
				{
					playerPressed.at(i) = false;
				}
				if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 22.0f)
				{
					won = true;
					for (int z = 0; z < gameObjects.size(); z++)
					{
						gameObjects.at(z)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
						if (z != i)
						{
							moonObjects.at(z)->GetComponent<Rigidbody>()->SetAcceleration(Vector4(0, -rand() % 15 -10, 0, 0));
						}
					}
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

void ButtonMashingScene::UIRender()
{
	ImGui::Begin("Game UI");
	
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > 22.0f)
		{
			std::string text  = "Player"+std::to_string(i+1)+" Wins";
			const char* textchar = text.c_str();
			
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
