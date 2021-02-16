#include "CollectingScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"
#include <math.h>
#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
CollectingScene::CollectingScene()
{
    MaxPlayers = 4;
    srand(time(NULL));
    for (int i = 0; i < MaxPlayers; i++)
    {
        OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 00.0f, -25.0f));
        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 2,0, 0, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
        playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 0, 1.4));
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, -5, 0, 0));
        playerScores.push_back(0);

    }
    for (int i = 0; i < MaxTargets; i++)
    {
        targetObjects.push_back(new GameObject());
        targetObjects.back()->AddComponent<Rigidbody>();
        targetObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20, 0, 0, 0));
        targetObjects.back()->AddComponent<RenderComponent>();
        targetObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 1, 1.4));
        objectSpawnTimes.push_back(0);
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
CollectingScene::~CollectingScene()
{

}
//float timeToStart;
bool CollectingScene::Initialize()
{
    return true;
}
 void CollectingScene::Update(const float deltaTime_)
{
     spawnObjects(deltaTime_);
     for (int i = 0; i < playerObjects.size(); i++)
     {
         playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -0.03, 0, 0));
         if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
         {
             if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX()>-maxVelocity)
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
             playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(-5, playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetY(), 0, 0) );
         }
     }
}

  void CollectingScene::UIRender()
{

}
  void CollectingScene::UpdateScore(int scorePosition_, int score_)
  {
      playerScores.at(scorePosition_) += score_;
      if (playerScores.at(scorePosition_) >= maxScore)
          won = true;
  }
  void CollectingScene::spawnObjects(float deltaTime_)
  {
      
      for (int i = 0; i < targetObjects.size(); i++)
      {
          if (objectSpawned.at(i) == false)
              objectSpawnTimes.at(i) -= deltaTime_;
              if (objectSpawnTimes.at(i) <= 0)
              {
                  objectSpawnTimes.at(i)= rand() % 4 + 1.5f;
                  objectSpawned.at(i) = true;
                  targetObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(rand() % (int)maxRight*2+minRight, rand() % (int)maxHeight, 0, 0));
              }
          }
  }