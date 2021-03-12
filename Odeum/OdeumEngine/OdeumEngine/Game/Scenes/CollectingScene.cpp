#include "CollectingScene.h"

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

CollectingScene::CollectingScene()
{
    OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 00.0f, -25.0f));

    MaxPlayers = 4;
    srand(time(NULL));
    for (int i = 0; i < MaxPlayers; i++)
    {
        playerObjects.push_back(new GameObject());
        playerObjects.back()->AddComponent<Rigidbody>();
        playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 4-6, rand()%3-1, 0, 0));
        playerObjects.back()->AddComponent<RenderComponent>();
       // playerObjects.back()->GetComponent<Rigidbody>()->SetMass(1.0f);

       // playerObjects.back()->GetComponent<Rigidbody>()->SetRadius(1.3f);
      //  playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 1));
       // playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::SphereShape, Colour(0, 0, 1.4));
        playerObjects.back()->AddComponent<ComplexCollider>();
        playerObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Chicken.obj");
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetRotation(Vector4(0, -1, 0, 0),180);
        playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, -5, 0, 0));
        playerScores.push_back(0);

    }

    for (int i = 0; i < MaxTargets; i++)
    {
        targetObjects.push_back(new GameObject());
        targetObjects.back()->AddComponent<Rigidbody>();
        targetObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20, 0, 0, 0));
        targetObjects.back()->AddComponent<RenderComponent>();
        //targetObjects.back()->GetComponent<Rigidbody>()->SetMass(1.0f);

       // targetObjects.back()->GetComponent<Rigidbody>()->SetRadius(1.3f);
       // targetObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 1));
        

        //targetObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0, 1, 1.4));
        targetObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/bread.obj");
        targetObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0, -1, 0, 0), 180);
        //targetObjects.back()->GetComponent<Rigidbody>()->SetScale
        targetObjects.back()->AddComponent<ComplexCollider>();
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
     if (!won)
     {
         spawnObjects(deltaTime_);

         for (int i = 0; i < playerObjects.size(); i++)
         {
             for (int z = 0; z < playerObjects.size(); z++)
             {
                 if (i != z)
                 {
                     if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), playerObjects.at(z)->GetComponent<ComplexCollider>(),simplex))
                     {
                         Debug::Warning("player overlap", __FILENAME__, __LINE__);
                         if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() > playerObjects.at(z)->GetComponent<Rigidbody>()->GetPosition().GetY())
                             playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 10, 0, 0));
                         else
                             playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), -10, 0, 0));
                     }
                 }
             }
            
             


             playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(0, -10*deltaTime_, 0, 0));
             if (Input::Get().isKeyPressed(playerKeysLeft.at(i)))
             {
                 if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() > -maxVelocity)
                     playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(-8*deltaTime_, 0, 0, 0));



             }
             else if (Input::Get().isKeyPressed(playerKeysRight.at(i)))
             {
                 if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX() < maxVelocity)
                     playerObjects.at(i)->GetComponent<Rigidbody>()->AddVelocity(Vector4(10* deltaTime_, 0, 0, 0));
             }

             if (playerObjects.at(i)->GetComponent<Rigidbody>()->GetPosition().GetY() < minHeight)
             {
                 playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 15, 0, 0));
                 if (Input::Get().isKeyPressed(playerKeysUp.at(i)))
                 {
                     playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 20, 0, 0));
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
             for (int x = 0; x < targetObjects.size(); x++)
             {
                 if (CollisionDetection::GJKCollisionDetection(playerObjects.at(i)->GetComponent<ComplexCollider>(), targetObjects.at(x)->GetComponent<ComplexCollider>(),simplex))
                 {
                     // playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(playerObjects.at(i)->GetComponent<Rigidbody>()->GetVelocity().GetX(), 10, 0, 0));
                     Debug::Warning("overlap", __FILENAME__, __LINE__);
                     targetObjects.at(x)->GetComponent<Rigidbody>()->SetPosition(Vector4(100, 100, 100, 0));
                     objectSpawned.at(x) = false;
                     UpdateScore(i, 1);
                 }


             }
         }
     }
}

  void CollectingScene::UIRender()
{
      ImGui::Begin("Game UI");
      ImGui::Text("Enter game UI components here");
      for (int i = 0; i < playerScores.size(); i++)
      {
          float test = playerScores.at(i);
          std::string text = "player "+ std::to_string(i) + " score: " + std::to_string(test);

          const char* textchar = text.c_str();
          ImGui::Text(textchar);
          if (test >= maxScore)
          {
              text = "player " + std::to_string(i) + " wins";
              textchar = text.c_str();
              ImGui::Text(textchar);
          }
      }
      ImGui::End();
}
  void CollectingScene::UpdateScore(int scorePosition_, int score_)
  {
      playerScores.at(scorePosition_) += score_;
      if (playerScores.at(scorePosition_) >= maxScore)
      {
          won = true;
          for (int i = 0; i < playerObjects.size(); i++)
          {
              playerObjects.at(i)->GetComponent<Rigidbody>()->SetVelocity(Vector4(0, 0, 0, 0));
          }
      }

  }
  void CollectingScene::spawnObjects(float deltaTime_)
  {
      
      for (int i = 0; i < targetObjects.size(); i++)
      {
          if (objectSpawned.at(i) == false)
              objectSpawnTimes.at(i) -= deltaTime_;
              if (objectSpawnTimes.at(i) <= 0)
              {
                  objectSpawnTimes.at(i)= rand() % 2 + 0.5f;
                  objectSpawned.at(i) = true;
                  targetObjects.at(i)->GetComponent<Rigidbody>()->SetPosition(Vector4(rand() % (int)maxRight*2+minRight, rand() % (int)maxHeight, 0, 0));
              }
          }
  }