#include "BattleScene.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
BattleScene::BattleScene()
{
    player1Object = new GameObject();
    player1Object->AddComponent<StatComponent>();
    player1=player1Object->GetComponent<StatComponent>();
    player2Object = new GameObject();
    player2Object->AddComponent<StatComponent>();
    player2 = player2Object->GetComponent<StatComponent>();
    //player1.SetAttack(20, 1, 1, 1, 1);
    //player2.SetAttack(20, 1, 1, 1, 1);
    player1Keys.push_back(Key::J);
    player1Keys.push_back(Key::K);
    player1Keys.push_back(Key::L);
    player2Keys.push_back(Key::A);
    player2Keys.push_back(Key::S);
    player2Keys.push_back(Key::D);
    srand(time(NULL));
    DecideFirstTurn(player1, player2);
    player1->SetCurrentStat(3, CombatStatTypes::Health);
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
                    DamageCalculation(player1, player2, player1Choice, player2Choice);
                    player1Turn = false;
                    Debug::Warning("player 1 attack", __FILENAME__, __LINE__);
                }
                else
                {
                    DamageCalculation(player2, player1, player2Choice, player1Choice);
                    player1Turn = true;
                    Debug::Warning("player 2 attack", __FILENAME__, __LINE__);
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
    float test = player1->GetStat(CombatStatTypes::Health).currentValue;
    std::string text = "player1 health: "+std::to_string(test);
    
    const char* textchar = text.c_str();
    ImGui::Text(textchar);
    test = player1->GetStat(CombatStatTypes::Defense).currentValue;
    text = "player1 defence: " + std::to_string(test);

    textchar = text.c_str();
    ImGui::Text(textchar);


     test = player2->GetStat(CombatStatTypes::Health).currentValue;
     text = "player2 health: " + std::to_string(test);

     textchar = text.c_str();
    ImGui::Text(textchar);
    test = player2->GetStat(CombatStatTypes::Defense).currentValue;
    text = "player2 defence: " + std::to_string(test);

    textchar = text.c_str();
    ImGui::Text(textchar);
    ImGui::End();
 }
void  BattleScene::DamageCalculation(StatComponent* attacker_, StatComponent* defender_, int attackType,int defenceType)
{ //1=rock 2=paper 3= scissors;
    srand(time(NULL));
    bool miss = false;
    double damage = rand() % damageFlux;
    damage = (damage / 100) + 1;
        switch (attackType) {
        case 1: 
           damage*= attacker_->GetStat(CombatStatTypes::RockAttack).currentValue;
            switch (defenceType)
            {
            case 1:
                
               
                break;
            case 2:
                damage *= lessDamageModifier;
                break;
            case 3:
                damage *= extraDamageModifier;
                break;
            default:
                break;
            }
            break;
        case 2: 
            damage *= attacker_->GetStat(CombatStatTypes::PaperAttack).currentValue;
            switch (defenceType)
            {
            case 1:
                damage *= extraDamageModifier;
                break;
           case 2:
               
                break;
            case 3:
                damage *= lessDamageModifier;
                break;
            default:
                break;
            }
            break;
        case 3:
            damage *= attacker_->GetStat(CombatStatTypes::ScissorsAttack).currentValue;
            switch (defenceType)
            {
            case 1:
                damage *= lessDamageModifier;
                break;
            case 2:
                damage *= extraDamageModifier;
                break;
            case 3:
               
           default:
                break;
            }
           break;
        default:
           break;
        }
        if (!miss)
        {
            if (damage<=0)
            {
                damage = 1;
                Debug::Warning("small dmg", __FILENAME__, __LINE__);
            }
         
            //defender_->TakeDamage(damage);
            Debug::Warning("damage: "+std::to_string(damage), __FILENAME__, __LINE__);
            TakeDamage(damage, defender_);
            if (defender_->GetStat(CombatStatTypes::Health).currentValue <= 0)
            {
                //die and end combat
            }
         }
        
        //std::string string = std::to_string(damage);
       // const char* textchar = string.c_str();
        //ImGui::Text(textchar);
        //Debug::Warning("combat", __FILENAME__, __LINE__);
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
void BattleScene::TakeDamage(double Damage, StatComponent* defender_)
{
    double defense = defender_->GetStat(CombatStatTypes::Defense).currentValue;

    if (Damage > defense)
    {
        defender_->SetCurrentStat(0, CombatStatTypes::Defense);

        Damage -= defense;
        if(defender_->GetStat(CombatStatTypes::Health).currentValue> Damage)
        defender_->ModifyCurrentStat(-Damage, CombatStatTypes::Health);
        else
        {
            defender_->SetCurrentStat(0, CombatStatTypes::Health);
        }

        if (defender_->GetStat(CombatStatTypes::Health).currentValue <= 0)
        {
            // die
        }
    }
    else
    {
        defender_->ModifyCurrentStat(-Damage, CombatStatTypes::Defense);
    }

}
