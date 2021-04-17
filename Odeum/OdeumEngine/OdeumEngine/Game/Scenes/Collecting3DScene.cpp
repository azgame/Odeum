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
#include"../../Engine/Core/AudioHandler.h"
#include "../../Engine/CoreComponents/AudioSource.h"
Collecting3DScene::Collecting3DScene()
{
    playerColors.push_back(&Colour(0, 0, 1, 0.5));
    playerColors.push_back(&Colour(1, 0, 0, 0.5));
    playerColors.push_back(&Colour(1, 1, 0, 0.5));
    playerColors.push_back(&Colour(0, 1, 0, 0.5));

    AudioHandler::GetInstance()->Initialize();
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
        //playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 0, 1.4));
        playerObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Chicken.obj");
        playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetRotation(Vector4(-1,0,0,0), 90);
        playerScore.push_back(0);
        playerObjects.back()->GetComponent<RenderComponent>()->SetColour(*playerColors.at(i));
    }
    playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(8, rand() % 4 - 1, -1, 0));
    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, -10, 2.5, 0));

    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(40, 30, 1, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cliff_block_rock.obj");
    backgroundObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(0, 0, 0, 0.2));

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 20, 0, 0));

    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(80, 60, 1, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/cliff_block_rock.obj");
    backgroundObjects.back()->GetComponent<RenderComponent>()->SetColour(Colour(0, 0, 0, 0.2));

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(10, 20, -25, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cloud1.fbx");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(5, 5,5, 1));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-13, 25, -24, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cloud2.fbx");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-45, 35, -22, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cloud2.fbx");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(45, 22, -18, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cloud3.fbx");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 28, 1, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0),90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(27, 22, 1, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10, 10, 10, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(23, 58, 1, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(14, 14, 14, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(23, 58, 1, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(14, 14, 14, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-16, 45, 1, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(11, 11, 11, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-8, 30, 1, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(9, 9, 9, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-32, 36, -3, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(9, 9, 9, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-2, 55, -3, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(11, 11, 11, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(9, 36, -3, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(-1, 0, 0, 0), 90.0f);
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(11, 11, 11, 1));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat.obj");

    for (int i = 0; i < MaxCollectables; i++)
    {
        collectableObjects.push_back(new GameObject());
        collectableObjects.back()->AddComponent<Rigidbody>();
        collectableObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(40, 0, 0, 0));
        collectableObjects.back()->AddComponent<RenderComponent>();
       // collectableObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 1, 1.4));
        collectableObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(6, 6, 6, 1));
        //collectableObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(1, 0, 0, 0), 90);
        collectableObjects.back()->GetComponent<Rigidbody>()->AddRotation(Vector4(0, -1, 0, 0), 90);
        collectableObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/bread.obj");
        collectableObjects.back()->AddComponent<ComplexCollider>();
        objectSpawnTimes.push_back(0);
        objectSpawned.push_back(false);
        collectableObjects.back()->AddComponent<AudioSource>();
        collectableObjects.back()->GetComponent<AudioSource>()->Initialize("HappySax.ogg", false, false, false, 0.5f);
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
    playerObjects.back()->AddComponent<AudioSource>();
    playerObjects.back()->GetComponent<AudioSource>()->Initialize("CanCan.mp3", true, false, false, 0.5f);
    playerObjects.back()->GetComponent<AudioSource>()->PlaySound();
}
Collecting3DScene::~Collecting3DScene()
{
    for (int i = 0; i < playerColors.size(); i++)
    {
        delete (playerColors[i]);
    }
    playerColors.clear();
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
            playerObjects.at(i)->GetComponent<Rigidbody>()->SetRotation(Vector4(-1, 0, 0, 0), 90);
            playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0, 10 * deltaTime_, 0));
            


              
                if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
                {
                    
                    Debug::Warning(std::to_string(playerObjects.at(i)->GetComponent<Rigidbody>()->GetRotation().Get3x3().GetX().GetY()), __FILENAME__, __LINE__);
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, -1, 0, 0), 90);
                  
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() > -maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-12 * deltaTime_, 0, 0, 0));



                }
                else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 1, 0, 0), 90);
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() < maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(12 * deltaTime_, 0, 0, 0));
                }
                if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
                {
                    if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 1, 0, 0), 45);
                    }
                    else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 1, 0, 0), -45);
                    }
                    else
                    {
                      
                    }
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() < maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 12 * deltaTime_, 0, 0));
                }
                else if (Input::Get().isKeyPressed(playerKeysDown.at(i)))
                {
                    if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 1, 0, 0), -45);
                    }
                    else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 1, 0, 0), 45);
                    }
                    else
                    {
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 1, 0, 0), 180);
                    }
                   
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY() > -maxVelocity)
                        playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -12 * deltaTime_, 0, 0));
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
                            playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity() / (1 + 2 * deltaTime_));
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
                    if (z != i)
                    {
                        if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), playerObjects.at(z)->GetComponent<ComplexCollider>(), simplex))
                        {
                            if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() > playerObjects.at(z)->GetComponent<Rigidbody>()->GetPosition().GetZ() - 0.1)
                                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 10, 0));
                            else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() < playerObjects.at(z)->GetComponent<Rigidbody>()->GetPosition().GetZ())
                                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), -10, 0));
                        }
                    }
                }
                for (int x = 0; x < collectableObjects.size(); x++)
                {
                    if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), collectableObjects.at(x)->GetComponent<ComplexCollider>(), simplex))
                    {

                        
                       
                       // playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                        collectableObjects.at(x)->GetComponent<AudioSource>()->PlaySound();
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

void Collecting3DScene::UIRender()
{
    ImGui::Begin("Game UI");

   
        for (int i = 0; i < playerScore.size(); i++)
        {
            float test = playerScore.at(i);
            if (!won)
            {
                std::string text = "player " + std::to_string(i+1) + " score: " + std::to_string(test);

                const char* textchar = text.c_str();
                ImGui::Text(textchar);
            }
            if (test >= maxScore)
            {
                std::string text = "player " + std::to_string(i+1) + " wins";
                const char*  textchar = text.c_str();
                ImGui::Text(textchar);
            }
        }
    
    ImGui::End();
}

void Collecting3DScene::spawnObjects(float deltaTime_)
{

    for (int i = 0; i < collectableObjects.size(); i++)
    {
        collectableObjects.at(i)->GetComponent<Rigidbody>()->AddRotation(Vector4(-1 * deltaTime_, 0, 0, 0), 1);
        if (objectSpawned.at(i) == false)
        {
            objectSpawnTimes.at(i) -= deltaTime_;
            
        }
        if (objectSpawnTimes.at(i) <= 0)
        {
            objectSpawnTimes.at(i) = rand() % 2 + 0.5f;
            objectSpawned.at(i) = true;
            collectableObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(rand() % (int)maxRight * 2+ (minRight*1), rand() % (int)maxHeight * 2 + (minHeight * 1)-1, rand() % (int)minUp*-0.6f-2 , 0));
        }
    }
}