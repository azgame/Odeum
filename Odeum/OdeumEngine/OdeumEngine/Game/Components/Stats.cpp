#include "Stats.h"
void Stats::OnAttach(GameObject* parent)
{

}

void Stats::Update(float deltaTime)
{

}
void Stats::SetHealth(float staticHealthIncrease_, float healthModifier_)
{
	float oldBasehealth = CurrentMaxHealth;
	CurrentMaxHealth += staticHealthIncrease_;
	CurrentMaxHealth *= healthModifier_;
	//set current health based on the diffrence between the two values
	if (oldBasehealth < CurrentMaxHealth)
	{
		currentHealth += (CurrentMaxHealth - oldBasehealth);
	}
	 if(currentHealth> CurrentMaxHealth)
	{
		 currentHealth= CurrentMaxHealth;
	}
}
void Stats::SetDefence(float staticDefenceIncrease_, float defenceModifier_)
{
	CurrentMaxDefence += staticDefenceIncrease_;
	CurrentMaxDefence *= defenceModifier_;
}
void Stats::SetSpeed(float staticSpeedIncrease_, float speedModifier_)
{
	currentSpeed += staticSpeedIncrease_;
	currentSpeed *= speedModifier_;
}
void Stats::SetAttack(float staticAttackIncrease_, float attackModifier_, float rockModifier_, float scissorsModifier_, float paperModifier_)
{
	if (rockModifier_ != 0)
		rockModifier *= rockModifier_;
	if (paperModifier_ != 0)
		paperModifier *= paperModifier_;
	if (scissorsModifier_ != 0)
		scissorsModifier *= scissorsModifier_;
	currentAttack += staticAttackIncrease_;
	currentAttack *= attackModifier_;
}
void Stats::TakeDamage(float damage_)
{
	if (currentDefence > 0)
	{
		currentDefence -= damage_;
		if (currentDefence < 0)
			currentHealth += currentDefence;
	}
	else
	{
		currentHealth -= damage_;
	}

	
}