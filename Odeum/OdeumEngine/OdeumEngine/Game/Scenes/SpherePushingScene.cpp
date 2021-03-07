#include "SpherePushingScene.h"
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

SpherePushingScene::SpherePushingScene()
{
    MaxPlayers = 4;
    srand(time(NULL));
    OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, -45.0f, -20.0f));
    OdeumEngine::Get().GetCamera().SetLookDirection(Vector3(0.0f, 45.0f, 0.0f), Vector3(0.0f, 1, 0.0f));
    for (int i = 0; i < MaxPlayers; i++)
    {

        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 6 - 10, rand() % 4 - 1, 0, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetMass(1.0f);
        
        playerObjects.back()->GetComponent<Rigidbody>()->SetRadius(1.3f);
        playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5,0.5,0.5,1));
        playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::SphereShape, Colour(0, 0, 1.4));
        playerObjects.back()->AddComponent<SphereCollider>();
        
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(1, 0, 0, 0));
        playerDead.push_back(false);

    }
   
   /* backgroundObjects.push_back(new GameObject());
    backgroundObjects.back()->AddComponent<Rigidbody>();
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0, 0, 2.5, 0));
    backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(18.5, 10, 1, 0));
    backgroundObjects.back()->AddComponent<RenderComponent>();
    backgroundObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1, 1, 0.2f));
    */


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
SpherePushingScene::~SpherePushingScene()
{

}
//float timeToStart;
bool SpherePushingScene::Initialize()
{
    return true;
}
void SpherePushingScene::Update(const float deltaTime_)
{
    if (!won)
    {
        //std::string r = std::to_string(playerObjects.at(0)->GetPosition().GetX());
       // Debug::Warning(r, __FILENAME__, __LINE__);
        /*if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(0)->GetComponent<SphereCollider>(), playerObjects.at(1)->GetComponent<SphereCollider>()))
        {
            Debug::Warning("collision", __FILENAME__, __LINE__);
       }*/
    }
    if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(0)->GetComponent<SphereCollider>(), playerObjects.at(1)->GetComponent<SphereCollider>()))
    {
        Vector4 V1 = playerObjects.at(0)->GetComponent<Rigidbody>()->GetVelocity();
        playerObjects.at(0)->GetComponent<Rigidbody>()->SetVelocity(playerObjects.at(1)->GetComponent<Rigidbody>()->GetVelocity());
        playerObjects.at(1)->GetComponent<Rigidbody>()->SetVelocity(V1 );
       
    }
    for (int i = 0; i < playerObjects.size(); i++)
    {
        if (!playerDead.at(i))
        {






            if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-0.01, 0, 0, 0));
            }
            else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0.01, 0, 0, 0));
            }
            if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, 0.01, 0, 0));
            }
            else if (Input::Get().isKeyPressed(playerKeysDown.at(i)))
            {
                playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.01, 0, 0));
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
            if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < minHeight || playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > maxHeight || playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() < minRight || playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetX() > maxRight)
            {
                playerDead.at(i) = true;
                playerObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(100, 100, 100, 0));
                playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
                for (int E = 0; E < playerDead.size(); E++)
                {
                    if (playerDead.at(E) == false)
                    {
                        continue;
                    }
                    else if (E == MaxPlayers - 1)
                    {
                        won = true;
                    }
                }


            }
                


                /*for (int z = 0; z < playerObjects.size(); z++)
                {
                    if (z != i)
                    {
                        if (CollisionDetection::SphereSphereCollisionDetection(playerObjects.at(i)->GetComponent<SphereCollider>(), playerObjects.at(z)->GetComponent<SphereCollider>()))
                        {
                            Debug::Warning("collision", __FILENAME__, __LINE__);
                            //CollisionHandler::GetInstance()->SphereSphereCollisionResponse(*playerObjects.at(i)->GetComponent<SphereCollider>(), *playerObjects.at(i)->GetComponent<SphereCollider>());
                            playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(playerObjects.at(z)->GetComponent<Rigidbody>()->GetVelocity());
                            //won = true;
                        }
                    }
                } */


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
            }
    }
}

void SpherePushingScene::UIRender()
{

}

