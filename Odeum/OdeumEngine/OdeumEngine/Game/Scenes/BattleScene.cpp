#include "BattleScene.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
BattleScene::BattleScene()
{
    player1 = StatComponent();
    //player1.SetAttack(20, 1, 1, 1, 1);
    //player2.SetAttack(20, 1, 1, 1, 1);
    player2 = StatComponent();
    player1Keys.push_back(Key::J);
    player1Keys.push_back(Key::K);
    player1Keys.push_back(Key::L);
    player2Keys.push_back(Key::A);
    player2Keys.push_back(Key::S);
    player2Keys.push_back(Key::D);
    srand(time(NULL));
    DecideFirstTurn(&player1, &player2);
}
BattleScene::~BattleScene()
{

}

bool BattleScene::Initialize()
{
    return false;
 }
void BattleScene::Update(const float deltaTime_)
{
    waitTime -= deltaTime_;
    if (waitTime <= 0)
    {
        if (!NPCBattle)
        {
            // Debug::Warning("! npc battle", __FILENAME__, __LINE__);
            if (player1Choice == 0)
            {
                for (int i = 0; i < player1Keys.size(); i++)
                {
                    if (Input::Get().isKeyPressed(player1Keys.at(i)))
                    {
                        player1Choice = i + 1;
                    }
                }
            }
            if (player2Choice == 0)
            {
                for (int i = 0; i < player2Keys.size(); i++)
                {
                    if (Input::Get().isKeyPressed(player2Keys.at(i)))
                    {
                        player2Choice = i + 1;
                    }
                }
            }
            if (player1Choice != 0 && player2Choice != 0)
            {
                Debug::Warning("combat", __FILENAME__, __LINE__);
                if (player1Turn)
                {
                    DamageCalculation(&player1, &player2, player1Choice, player2Choice);
                    player1Turn = false;
                }
                else
                {
                    DamageCalculation(&player2, &player1, player2Choice, player1Choice);
                    player1Turn = true;
                }
                player1Choice = player2Choice = 0;
                waitTime = 2.0f;
            }

        }
    }
 }
void BattleScene::UIRender()
{
    ImGui::Begin("Game UI");
    ImGui::Text("Enter game UI components here");

    //std::string text = "p1health " + std::to_string(player1.GetCurrentHealth());
    /*const char* textchar = text.c_str();
    ImGui::Text(textchar);*/
    ImGui::End();
 }
void  BattleScene::DamageCalculation(StatComponent* attacker_, StatComponent* defender_, int attackType,int defenceType)
{ //1=rock 2=paper 3= scissors;
    bool miss = false;
    float damage = 0;
        //switch (attackType) {
        //case 1: 
        //    switch (defenceType)
        //    {
        //    case 1:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetRockModifier();
        //        break;
        //    case 2:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetRockModifier() * lessDamageModifier;
        //        break;
        //    case 3:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetRockModifier() * extraDamageModifier;
        //        break;
        //    default:
        //        break;
        //    }
        //    break;
        //case 2: 
        //    switch (defenceType)
        //    {
        //    case 1:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetpaperModifier() * extraDamageModifier;
        //        break;
        //    case 2:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetpaperModifier();
        //        break;
        //    case 3:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetpaperModifier() * lessDamageModifier;
        //        break;
        //    default:
        //        break;
        //    }
        //    break;
        //case 3:
        //    switch (defenceType)
        //    {
        //    case 1:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetScissorsModifier() * lessDamageModifier;
        //        break;
        //    case 2:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetScissorsModifier() * extraDamageModifier;
        //        break;
        //    case 3:
        //         damage = attacker_->GetCurrentAttack() * attacker_->GetScissorsModifier();
        //    default:
        //        break;
        //    }
        //    break;
        //default:
        //    break;
        //}
        //if (!miss)
        //{
        //    defender_->TakeDamage(damage);
        //    if (defender_->GetCurrentHealth() <= 0)
        //    {
        //        //die and end combat
        //    }
        // }
       // std::string string = defender_->GetCurrentHealth()->toString();
        Debug::Warning("combat", __FILENAME__, __LINE__);
}
void BattleScene::DecideFirstTurn(StatComponent* player1_, StatComponent* player2_)
{
    float randomNumber = rand() % 100;
    /*float p1chance =50+ player1_->GetCurrentSpeed() - player2_->GetCurrentSpeed();
    if (p1chance > 80)
        p1chance = 80;
    if (p1chance < 20)
        p1chance = 20;
    if (p1chance <= randomNumber)
    {
        player1Turn = false;
    }
    else
    {
        player1Turn = true;
    }*/
    //1 extra point in speed is 1%chance to go first caps at 80%
}