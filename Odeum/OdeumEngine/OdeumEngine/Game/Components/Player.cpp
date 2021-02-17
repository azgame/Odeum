#include "Player.h"

void Player::OnAttach(GameObject* parent) 
{
	
}

void Player::OnStart()
{
	if (m_gameObject->HasComponent<StatComponent>())
		stats = m_gameObject->GetComponent<StatComponent>();
	else
	{
		m_gameObject->AddComponent<StatComponent>();
		stats = m_gameObject->GetComponent<StatComponent>();
	}
}

void Player::Update(float deltaTime)
{
	
}

void Player::TakeDamage(double Damage)
{
	double defense = stats->GetStat(CombatStatTypes::Defense).currentValue;

	if (Damage > defense)
	{
		stats->SetCurrentStat(0, CombatStatTypes::Defense);
		Damage -= defense;
		stats->ModifyCurrentStat(-Damage, CombatStatTypes::Health);

		if (stats->GetStat(CombatStatTypes::Health).currentValue <= 0)
		{
			// die
		}
	}
	else
	{
		stats->ModifyCurrentStat(-Damage, CombatStatTypes::Defense);
	}

}
