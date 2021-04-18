#include "AvoidScene.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../../Engine/Math/CollisionHandler.h"
#include "../../Engine/Math/Collider.h"
#include "../../Engine/Math/Plane.h"
#include "../../Engine/NavMesh/NavMeshManager.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

AvoidScene::AvoidScene()
{
    playerColors.push_back(&Colour(0, 0, 1, 0.5));
    playerColors.push_back(&Colour(1, 0, 0, 0.5));
    playerColors.push_back(&Colour(1, 1, 0, 0.5));
    playerColors.push_back(&Colour(0, 1, 0, 0.5));
    MaxPlayers = 4;
    srand(time(NULL));

    OdeumEngine::Get().GetCamera().SetEyeAtUp(
        Vector3(-2.5f, 50.0f, -5.0f), 
        Vector3(-2.5f, 0.0f, 0.0f), 
        Vector3(kYUnitVector));

    //OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0, 50.0, -5.0));

    for (int i = 0; i < MaxPlayers; i++)
    {
        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-10, 0, i * 4 - 8, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetMass(1.0f);

        playerObjects.back()->GetComponent<Rigidbody>()->SetRadius(1.3f);
        playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1, 1, 1, 1));
        //playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::SphereShape, Colour(0, 0, 1.4));
        playerObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/moon.obj");

        playerObjects.back()->AddComponent<SphereCollider>();

        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
        playerDead.push_back(false);
        playerLastVelocity.push_back(Vector4(0, 0, 0, 0));
        playerObjects.back()->GetComponent<RenderComponent>()->SetColour(*playerColors.at(i));
    }

    std::vector<Collider2D> obstacles;

    for (int i = 0; i < kMaxObstacles; i++)
    {
        obstacleObjects.push_back(new GameObject());

        obstacleObjects[i]->AddComponent<Rigidbody>();
        obstacleObjects[i]->GetComponent<Rigidbody>()->SetPosition(Vector4(-16 + (i * 3), 0.0f, -16 + (i * 4), 1.0f));
        obstacleObjects[i]->GetComponent<Rigidbody>()->SetRotation(Vector4(kYUnitVector), i * 45.0f);
        obstacleObjects[i]->AddComponent<RenderComponent>();
        obstacleObjects[i]->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 0.2, 0.2));
        obstacleObjects[i]->AddComponent<BoxCollider>();

        OrientedBoundingBox obb = *obstacleObjects[i]->GetComponent<BoxCollider>()->GetBoundingBox();

        Vector2 center = Vector2(obb.center.GetX(), obb.center.GetZ());
        Vector2 minmin = center - Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) - Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());
        Vector2 maxmin = center + Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) - Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());
        Vector2 minmax = center - Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) + Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());
        Vector2 maxmax = center + Vector2(Vector2(obb.basis.GetX().GetX(), obb.basis.GetX().GetZ()) * obb.halfExtents.GetX()) + Vector2(Vector2(obb.basis.GetZ().GetX(), obb.basis.GetZ().GetZ()) * obb.halfExtents.GetZ());

        Vector3 min = obb.GetMin();
        Vector3 max = obb.GetMax();

        std::vector<Vector2> objectOutline;
        objectOutline.push_back(minmin);
        objectOutline.push_back(maxmin);
        objectOutline.push_back(minmax);
        objectOutline.push_back(maxmax);

        obstacles.push_back(Collider2D(objectOutline));
    }

    Plane groundPlane;
    groundPlane.min = Vector3(minRight, 0.0f, minHeight);
    groundPlane.max = Vector3(maxRight, 0.0f, maxHeight);
    groundPlane.normal = Vector3(kYUnitVector);

    NavMeshManager::GenerateNavMesh(0.25f, groundPlane, obstacles);

    enemyObject = new GameObject();
    enemyObject->AddComponent<Rigidbody>();
    enemyObject->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 0, 0));
    enemyObject->GetComponent<Rigidbody>()->SetRadius(1.3f);
    enemyObject->GetComponent<Rigidbody>()->AddRotation(Vector4(1, 0, 0, 0), 180);
    enemyObject->GetComponent<Rigidbody>()->AddRotation(Vector4(0, 0, 1, 0), 180);
    enemyObject->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 1));
    enemyObject->AddComponent<RenderComponent>();
    enemyObject->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Ghost.fbx");
    enemyObject->AddComponent<SphereCollider>();

    backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5, -2, 0, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(25, 1, 20, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1, 0, 0.2f,0.5f));

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

AvoidScene::~AvoidScene()
{
    for (int i = 0; i < playerColors.size(); i++)
    {
        delete (playerColors[i]);
    }
    playerColors.clear();
}

bool AvoidScene::Initialize()
{
    return true;
}

void AvoidScene::Update(const float deltaTime_)
{
    if (!won)
    {
        timeToWin -= deltaTime_;
        

        for (int i = 0; i < playerObjects.size(); i++)
        {
            playerLastVelocity.at(i) = playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity();
        }
        for (int i = 0; i < playerObjects.size(); i++)
        {
            if (!playerDead.at(i))
            {






                if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-15 * deltaTime_, 0, 0, 0));
                }
                else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(15 * deltaTime_, 0, 0, 0));
                }
                if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0, 15 * deltaTime_, 0));
                }
                else if (Input::Get().isKeyPressed(playerKeysDown.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0, -15 * deltaTime_, 0));
                }

                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() < minRight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(10, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() > maxRight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-10, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() < minHeight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 10, 0));

                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetZ() > maxHeight)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), -10, 0));
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
                /*if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(0)->GetComponent<SphereCollider>(), playerObjects.at(1)->GetComponent<SphereCollider>()))
                {
                    Vector4 V1 = playerObjects.at(0)->GetComponent<Rigidbody>()->GetVelocity();
                    playerObjects.at(0)->GetComponent<Rigidbody>()->SetVelocity(playerObjects.at(1)->GetComponent<Rigidbody>()->GetVelocity() * 1.1f);
                    playerObjects.at(1)->GetComponent<Rigidbody>()->SetVelocity(V1 * 1.1f);

                }*/

                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ() < -maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -maxVelocity, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ() > maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), maxVelocity, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() > maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(maxVelocity, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() < -maxVelocity)
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-maxVelocity, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ(), 0));
                }
                /* if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ() > maxVelocity)
                 {
                     playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), maxVelocity, 0));
                 }
                 else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetZ() < -maxVelocity)
                 {
                     playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), -maxVelocity, 0));
                 }*/
                if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(i)->GetComponent<SphereCollider>(), enemyObject->GetComponent<SphereCollider>()))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(100, 100, 100, 0));
                    playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                    playerDead.at(i) = true;
                    currentDeadPlayers += 1;

                    if (currentDeadPlayers >= MaxPlayers-1)
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
        if (timeToWin <= 0)
        {
            won = true;
            for (int w = 0; w < playerObjects.size(); w++)
            {
                playerObjects.at(w)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
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
                //return who wins
            }
        }
    }
}

void AvoidScene::UIRender()
{
    ImGui::Begin("Game UI");
    ImGui::Text("Push everyone off");
    if (won)
    {
        for (int i = 0; i < playerObjects.size(); i++)
        {
            if (playerDead.at(i) == false)
            {
                std::string text = "Player" + std::to_string(i + 1) + " Wins ";
                const char* textchar = text.c_str();
                ImGui::Text(textchar);
            }

        }
    }
    ImGui::End();
}

