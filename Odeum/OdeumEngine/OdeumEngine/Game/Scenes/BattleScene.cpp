#include "BattleScene.h"
BattleScene::BattleScene(Stats p1_, Stats p2_)
{
    player1 = p1_;
    player2 = p2_;
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

 }
void BattleScene::UIRender()
{

 }
void  BattleScene::DamageCalculation(Stats*attacker_,Stats*defender_, int attackType,int defenceType)
{ //1=rock 2=paper 3= scissors;
    bool miss = false;
    float damage = 0;
        switch (attackType) {
        case 1: 
            switch (defenceType)
            {
            case 1:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetRockModifier();
                break;
            case 2:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetRockModifier() * lessDamageModifier;
                break;
            case 3:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetRockModifier() * extraDamageModifier;
                break;
            default:
                break;
            }
            break;
        case 2: 
            switch (defenceType)
            {
            case 1:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetpaperModifier() * extraDamageModifier;
                break;
            case 2:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetpaperModifier();
                break;
            case 3:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetpaperModifier() * lessDamageModifier;
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (defenceType)
            {
            case 1:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetScissorsModifier() * lessDamageModifier;
                break;
            case 2:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetScissorsModifier() * extraDamageModifier;
                break;
            case 3:
                 damage = attacker_->GetCurrentAttack() * attacker_->GetScissorsModifier();
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (!miss)
        {
            defender_->TakeDamage(damage);
            if (defender_->GetCurrentHealth() <= 0)
            {
                //die and end combat
            }
         }
}