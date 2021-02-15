#include "StatComponent.h"

#include "../../Engine/JSONUtility/json.hpp"

void StatComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;

	std::ifstream configFile("Game/GameAssets/PlayerStats.json");
	nlohmann::json jsonReader;

	configFile >> jsonReader;
	configFile.close();

	for (const auto& item : jsonReader.items())
	{
		std::cout << item.key() << "\n";
		for (const auto& val : item.value().items())
		{
			std::cout << "  " << val.key() << ": " << val.value() << "\n";
		}
	}
}

void StatComponent::Update(float deltaTime)
{
}

void StatComponent::SetHealth(float staticHealthIncrease_, float healthModifier_)
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

void StatComponent::SetDefence(float staticDefenceIncrease_, float defenceModifier_)
{
	CurrentMaxDefence += staticDefenceIncrease_;
	CurrentMaxDefence *= defenceModifier_;
}

void StatComponent::SetSpeed(float staticSpeedIncrease_, float speedModifier_)
{
	currentSpeed += staticSpeedIncrease_;
	currentSpeed *= speedModifier_;
}

void StatComponent::SetAttack(float staticAttackIncrease_, float attackModifier_, float rockModifier_, float scissorsModifier_, float paperModifier_)
{
	if (rockModifier_ != 0)
		rockModifier += rockModifier_;
	if (paperModifier_ != 0)
		paperModifier += paperModifier_;
	if (scissorsModifier_ != 0)
		scissorsModifier += scissorsModifier_;
	currentAttack += staticAttackIncrease_;
	currentAttack *= attackModifier_;
}

void StatComponent::TakeDamage(float damage_)
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

bool StatComponent::HasModifier(UINT16 Id, PlayerStatTypes Type)
{
	if (Type != PlayerStatTypes::None)
	{

	}
	else
	{

	}

	return false;
}

void StatComponent::RegisterLinearModifier(PlayerStatMod Mod)
{
}

void StatComponent::RegisterMulModifier(PlayerStatMod Mod)
{
}

double StatComponent::GetModifier(UINT16 Id, PlayerStatTypes Type)
{
	if (Type != PlayerStatTypes::None)
	{

	}
	else
	{

	}
}

PlayerStat StatComponent::GetStat(PlayerStatTypes Type)
{
	return PlayerStat();
}

void StatComponent::SetStat(PlayerStatTypes Type, uint32_t NewValue)
{
}

void StatComponent::ModifyStat(PlayerStatTypes Type, uint32_t Value)
{
}

void StatComponent::ResetStat(PlayerStatTypes Type)
{
}

void StatComponent::RecalculateStat()
{
}

