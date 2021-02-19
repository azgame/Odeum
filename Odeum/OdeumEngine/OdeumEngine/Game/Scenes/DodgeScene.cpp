#include "DodgeScene.h"


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

DodgeScene::DodgeScene()
{
    MaxPlayers = 4;
    srand(time(NULL));
    for (int i = 0; i < MaxPlayers; i++)
    {
        OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 00.0f, -25.0f));
        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 4 - 6, rand() % 3 - 1, 0, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
        playerObjects.back()->AddComponent<ComplexCollider>();
        playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 0, 1.4));
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, -5, 0, 0));
        playerDead.push_back(false);

    }
    for (int i = 0; i < MaxObstacles; i++)
    {
        obstacleObjects.push_back(new GameObject());
        obstacleObjects.back()->AddComponent<Rigidbody>();
        obstacleObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20, 0, 0, 0));
        obstacleObjects.back()->AddComponent<RenderComponent>();
        obstacleObjects.back()->AddComponent<ComplexCollider>();
        obstacleObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(2.1f, 0, 0));
        objectSpawnTimes.push_back(rand() % 10 + 0.5f);
        
        objectSpawned.push_back(false);
    }
    playerKeysLeft.push_back(Key::A);
    playerKeysRight.push_back(Key::S);

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
 DodgeScene::~DodgeScene()
{

}
//float timeToStart;
bool DodgeScene::Initialize()
{
    return true;
}
void DodgeScene::Update(const float deltaTime_)
{
    if (!won)
    {
        spawnObjects(deltaTime_);

        for (int i = 0; i < playerObjects.size(); i++)
        {
            for (int z = 0; z < playerObjects.size(); z++)
            {
                if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), playerObjects.at(z)->GetComponent<ComplexCollider>(), simplex))
                {
                    if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > playerObjects.at(z)->GetComponent<Rigidbody>()->GetPosition().GetY())
                        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 10, 0, 0));
                    else
                        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -10, 0, 0));
                }
            }

            for (int x = 0; x < obstacleObjects.size(); x++)
            {
              //  if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), obstacleObjects.at(x)->GetComponent<ComplexCollider>(), simplex))
               // {
                    // playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 10, 0, 0));
               //     Debug::Warning("overlap", __FILENAME__, __LINE__);
             //       targetObjects.at(x)->GetComponent<Rigidbody>()->SetPosition(Vector4(100, 100, 100, 0));
             //       objectSpawned.at(x) = false;
             //       UpdateScore(i, 1);
             //   }


            }


            playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.03, 0, 0));
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

            if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < minHeight)
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 15, 0, 0));
                if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
                {
                    playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 5, 0, 0));
                }
            }
            else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > maxHeight)
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -5, 0, 0));
            }
            if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() < minRight)
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity((Vector4(5, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 0, 0)));
            }
            else if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() > maxRight)
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-5, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 0, 0));
            }
        }
    }
}

void DodgeScene::UIRender()
{

}

void DodgeScene::spawnObjects(float deltaTime_)
{

    for (int i = 0; i < obstacleObjects.size(); i++)
    {
        if (obstacleObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() > 25)
        {
            objectSpawned.at(i) = false;
        }
        if (objectSpawned.at(i) == false)
            objectSpawnTimes.at(i) -= deltaTime_;
        if (objectSpawnTimes.at(i) <= 0)
        {
            objectSpawnTimes.at(i) = rand() % 3 + 0.5f;
            objectSpawned.at(i) = true;
            obstacleObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(minRight, rand() % (int)maxHeight*2+minHeight, 0, 0));
            obstacleObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(rand() % 6 + 2.5f, 0, 0, 0));
        }
    }
}