#include "TreadmillScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../../Engine/Math/CollisionHandler.h"
#include "../../Engine/Math/Collider.h"
#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

TreadmillScene::TreadmillScene()
{
    MaxPlayers = 4;
    timeToStart = 4;
    srand(time(NULL));
    OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, -45.0f, -20.0f));
    OdeumEngine::Get().GetCamera().SetLookDirection(Vector3(0.0f, 45.0f, 0.0f), Vector3(0.0f, 1, 0.0f));
    for (int i = 0; i < MaxPlayers; i++)
    {

        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, i * 4 - 8, 0, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetMass(1.0f);

        playerObjects.back()->GetComponent<Rigidbody>()->SetRadius(1.3f);
        playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.3, 0.3, 0.3, 1));
        //playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::SphereShape, Colour(0, 0, 1.4));
        playerObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/pig.obj");
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 90);
        playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), -90);
        playerObjects.back()->AddComponent<SphereCollider>();

        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(1, 0, 0, 0));
        playerDead.push_back(false);
        playerLastVelocity.push_back(Vector4(0, 0, 0, 0));

    }
    for (int i = 0; i < MaxObstacles; i++)
    {
        obstacleObjects.push_back(new GameObject());
        obstacleObjects.back()->AddComponent<Rigidbody>();
        obstacleObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(40, 0, 0, 0));
        obstacleObjects.back()->GetComponent<Rigidbody>()->SetRadius(1.3f);
        obstacleObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 1));
        
        obstacleObjects.back()->AddComponent<RenderComponent>();
        //obstacleObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::SphereShape, Colour(2.1f, 0, 0));
        obstacleObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cow.obj");
        obstacleObjects.back()->AddComponent<SphereCollider>();
        obstacleObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, 1, 0, 0), 270);
        obstacleObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 90);
      
        objectSpawnTimes.push_back(rand() % 20 + 1.8f);
        objectSpawned.push_back(false);
        
    }
    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 2.5, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(17.5, 9, 1, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1, 1, 0.2f));



    playerKeysLeft.push_back(Key::A);
    playerKeysRight.push_back(Key::D);
    playerKeysDown.push_back(Key::S);
    playerKeysDown.push_back(Key::M);
    playerKeysDown.push_back(Key::M);
    playerKeysDown.push_back(Key::M);


    playerKeysLeft.push_back(Key::H);
    playerKeysRight.push_back(Key::J);
    playerKeysLeft.push_back(Key::H);
    playerKeysRight.push_back(Key::J);
    playerKeysLeft.push_back(Key::H);
    playerKeysRight.push_back(Key::J);
    playerKeysUp.push_back(Key::W);
    playerKeysUp.push_back(Key::U);
    playerKeysUp.push_back(Key::U);
    playerKeysUp.push_back(Key::U);

}
TreadmillScene::~TreadmillScene()
{

}
//float timeToStart;
bool TreadmillScene::Initialize()
{
    return true;
}
void TreadmillScene::Update(const float deltaTime_)
{
    if (timeToStart <= 0)
    {
        spawnObjects(deltaTime_);
        for (int i = 0; i < playerObjects.size(); i++)
        {
            playerLastVelocity.at(i) = playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity();
        }
        for (int i = 0; i < playerObjects.size(); i++)
        {
            if (!playerDead.at(i))
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-10 * deltaTime_, 0, 0, 0));
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() > 0)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-10 * deltaTime_, 0, 0, 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() > 0)
                {
                  playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -5* deltaTime_, 0, 0));
                }
                else 
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 5 * deltaTime_, 0, 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() > -0.2 && playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() < 0.2)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 0, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(),0));
                }



                if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-5 * deltaTime_, 0, 0, 0));
                }
                else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(35 * deltaTime_, 0, 0, 0));
                }
                if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 600 * deltaTime_, 0, 0));
                }
                else if (Input::Get().isKeyPressed(playerKeysDown.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -600 * deltaTime_, 0, 0));
                }

                /* if (!Input::Get().isKeyPressed(playerKeysDown.at(i)) && !Input::Get().isKeyPressed(playerKeysUp.at(i)) && !Input::Get().isKeyPressed(playerKeysLeft.at(i)) && !Input::Get().isKeyPressed(playerKeysRight.at(i)) && !Input::Get().isKeyPressed(playerKeysJump.at(i)))
                 {
                     if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() >= 0)
                     {
                         if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().Mag() <= 1)
                         {
                             playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                         }
                         else
                         {
                             playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity() / 1.003);
                         }
                     }
                 }*/
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() < minRight)
                {
                    playerDead.at(i) = true;
                    //playerObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(100, 100, 100, 0));
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity() + Vector4(0, 0, 10, 0));
                   


                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() > maxRight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-10, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < minHeight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 10, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));

                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > maxHeight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -10, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }




                for (int z = 0; z < playerObjects.size(); z++)
                {
                    if (z != i)
                    {
                        if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(i)->GetComponent<SphereCollider>(), playerObjects.at(z)->GetComponent<SphereCollider>()))
                        {
                            CollisionHandler::GetInstance()->SphereSphereCollisionResponse(*playerObjects.at(i)->GetComponent<SphereCollider>(), *playerObjects.at(z)->GetComponent<SphereCollider>(), false);
                           

                        }
                    }
                }
               

                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() < -maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -maxVelocity, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() > maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), maxVelocity, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() > maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(maxVelocity , playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() < -maxVelocity*2)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-maxVelocity*2 , playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                
                /* if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ() > maxVelocity)
                 {
                     playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), maxVelocity, 0));
                 }
                 else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ() < -maxVelocity)
                 {
                     playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), -maxVelocity, 0));
                 }*/
                for (int x = 0; x < obstacleObjects.size(); x++)
                {
                    if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(i)->GetComponent<SphereCollider>(), obstacleObjects.at(x)->GetComponent<SphereCollider>()))
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity()+ obstacleObjects.at(x)->GetComponent<Rigidbody>()->GetVelocity()+Vector4(-75,0,0,0));
                    }
                }
                if (playerDead.at(i) == true)
                {
                    currentDeadPlayers += 1;
                    if (currentDeadPlayers >= MaxPlayers)
                    {
                        won = true;
                        Debug::Warning("win", __FILENAME__, __LINE__);
                        for (int t = 0; t < obstacleObjects.size(); t++)
                        {
                            obstacleObjects.at(t)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                        }
                        for (int w = 0; w < playerObjects.size(); w++)
                        {
                            playerObjects.at(w)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                        }
                    }
                }
            }
        }
    }
    else
    {
    timeToStart -= deltaTime_;
    }
}

void TreadmillScene::UIRender()
{
    ImGui::Begin("Game UI");
    ImGui::Text("Enter game UI components here");
    if (timeToStart > 0)
    {
        ImGui::Text("wait for it");

        std::string timeText = std::to_string(ceil(timeToStart));
        const char* timechar = timeText.c_str();

        ImGui::Text(timechar);
    }
    ImGui::End();
}
void TreadmillScene::spawnObjects(float deltaTime_)
{

    for (int i = 0; i < obstacleObjects.size(); i++)
    {
        if (obstacleObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() < -25)
        {
            objectSpawned.at(i) = false;
        }
        if (objectSpawned.at(i) == false)
            objectSpawnTimes.at(i) -= deltaTime_;
        if (objectSpawnTimes.at(i) <= 0)
        {
            objectSpawnTimes.at(i) = rand() % 3 + 0.5f;
            objectSpawned.at(i) = true;
            obstacleObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(maxRight, rand() % (int)maxHeight * 2 + minHeight, 0, 0));
            obstacleObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4( -3- rand() % 2, 0, 0, 0));
        }
    }
}

