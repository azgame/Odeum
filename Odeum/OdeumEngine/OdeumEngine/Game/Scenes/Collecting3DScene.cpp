#include "Collecting3DScene.h"




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

Collecting3DScene::Collecting3DScene()
{
    MaxPlayers = 4;
    srand(time(NULL));
    OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, -45.0f, -20.0f));
    OdeumEngine::Get().GetCamera().SetLookDirection(Vector3(0.0f, 45.0f, 0.0f), Vector3(0.0f, 1, 0.0f));
    for (int i = 0; i < MaxPlayers; i++)
    {

        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 4 - 6, rand() % 4 - 1, 0, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
        playerObjects.back()->AddComponent<ComplexCollider>();
        playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 0, 1.4));
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
        playerScore.push_back(0);

    }
    playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(8, rand() % 4 - 1, -1, 0));
    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 2.5, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(18.5, 10, 1, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1, 1, 0.2f));

    for (int i = 0; i < MaxCollectables; i++)
    {
        collectableObjects.push_back(new GameObject());
        collectableObjects.back()->AddComponent<Rigidbody>();
        collectableObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(40, 0, 0, 0));
        collectableObjects.back()->AddComponent<RenderComponent>();
        collectableObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 1, 1.4));
        collectableObjects.back()->AddComponent<ComplexCollider>();
        objectSpawnTimes.push_back(0);
        objectSpawned.push_back(false);
        
    }
   
    playerKeysLeft.push_back(Key::A);
    playerKeysRight.push_back(Key::D);
    playerKeysDown.push_back(Key::S);
    playerKeysDown.push_back(Key::M);
    playerKeysDown.push_back(Key::M);
    playerKeysDown.push_back(Key::M);
    playerKeysJump.push_back(Key::Space);
    playerKeysJump.push_back(Key::O);
    playerKeysJump.push_back(Key::O);
    playerKeysJump.push_back(Key::O);

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
Collecting3DScene::~Collecting3DScene()
{

}
//float timeToStart;
bool Collecting3DScene::Initialize()
{
    return true;
}
void Collecting3DScene::Update(const float deltaTime_)
{
    if (!won)
    {
        spawnObjects(deltaTime_);

        for (int i = 0; i < playerObjects.size(); i++)
        {
            




                playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0, 0.03, 0));

                if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
                {
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() > -maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-0.1, 0, 0, 0));



                }
                else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
                {
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() < maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0.1, 0, 0, 0));
                }
                if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
                {
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() < maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0.1, 0, 0));
                }
                else if (Input::Get().isKeyPressed(playerKeysDown.at(i)))
                {
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() > -maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.1, 0, 0));
                }
                if (!Input::Get().isKeyPressed(playerKeysDown.at(i)) && !Input::Get().isKeyPressed(playerKeysUp.at(i)) && !Input::Get().isKeyPressed(playerKeysLeft.at(i)) && !Input::Get().isKeyPressed(playerKeysRight.at(i)) && !Input::Get().isKeyPressed(playerKeysJump.at(i)))
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
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < minHeight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 2, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));

                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > maxHeight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -2, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() < minRight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity((Vector4(2, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0)));
                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() > maxRight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-2, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() < minUp)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 2, 0));

                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() > maxUp)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY(), 0, 0));
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 0, 0));
                    if (Input::Get().isKeyPressed(playerKeysJump.at(i)))
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), -10, 0));
                    }
                }

                for (int z = 0; z < playerObjects.size(); z++)
                {
                    if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), playerObjects.at(z)->GetComponent<ComplexCollider>(), simplex))
                    {
                        if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() > playerObjects.at(z)->GetComponent<Rigidbody>()->GetPosition().GetZ() - 0.1)
                            playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 10, 0));
                        else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() < playerObjects.at(z)->GetComponent<Rigidbody>()->GetPosition().GetZ())
                            playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), -10, 0));
                    }
                }
                for (int x = 0; x < collectableObjects.size(); x++)
                {
                    if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), collectableObjects.at(x)->GetComponent<ComplexCollider>(), simplex))
                    {

                        
                       
                       // playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                        playerScore.at(i) += 1;
                        collectableObjects.at(x)->GetComponent<Rigidbody>()->SetPosition(Vector4(100, 100, 100, 0));
                        objectSpawnTimes.at(x) = rand() % 2 + 0.5f;
                           
                        objectSpawned.at(x) = false;
                        if (maxScore <= playerScore.at(i))
                        {
                            won = true;
                            Debug::Warning("win", __FILENAME__, __LINE__);
                            for (int w = 0; w < playerObjects.size(); w++)
                            {
                                playerObjects.at(w)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                            }
                        }

                    }


                }
            
        }
    }
}

void Collecting3DScene::UIRender()
{

}

void Collecting3DScene::spawnObjects(float deltaTime_)
{

    for (int i = 0; i < collectableObjects.size(); i++)
    {
        if (objectSpawned.at(i) == false)
            objectSpawnTimes.at(i) -= deltaTime_;
        if (objectSpawnTimes.at(i) <= 0)
        {
            objectSpawnTimes.at(i) = rand() % 2 + 0.5f;
            objectSpawned.at(i) = true;
            collectableObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(rand() % (int)maxRight * 2+ (minRight*1), rand() % (int)maxHeight * 2 + (minHeight * 1), rand() % (int)minUp*-0.6f-2 , 0));
        }
    }
}