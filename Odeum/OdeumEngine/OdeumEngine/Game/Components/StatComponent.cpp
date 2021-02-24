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

void StatComponent::OnStart()
{
}

void StatComponent::Update(float deltaTime)
{
}

bool StatComponent::HasModifier(uint32_t Id, PlayerStatTypes Type)
{
	if (Type != PlayerStatTypes::None)
	{
		for (auto mod : m_modifiers[Type])
		{
			if (mod.id == Id)
				return true;
		}
	}
	else
	{
		for (auto modtypes : m_modifiers)
		{
			for (auto mod : modtypes.second)
				if (mod.id == Id)
					return true;
		}
	}

	return false;
}

void StatComponent::AddModifier(PlayerStatMod Mod)
{
	if (Mod.isMultiplicative)
		m_modifiers[Mod.type].push_back(Mod);
	else
		m_modifiers[Mod.type].insert(m_modifiers[Mod.type].begin(), Mod);

	RecalculateStat(Mod.type);
}

PlayerStatMod StatComponent::GetModifier(uint32_t Id, PlayerStatTypes Type)
{
	if (Type != PlayerStatTypes::None)
	{
		for (auto mod : m_modifiers[Type])
		{
			if (mod.id == Id)
				return mod;
		}
	}
	else
	{
		for (auto modtypes : m_modifiers)
		{
			for (auto mod : modtypes.second)
				if (mod.id == Id)
					return mod;
		}
	}

	PlayerStatMod noMod;
	noMod.id = -1;
	noMod.value = -1;
	noMod.type = PlayerStatTypes::None;

	return noMod;
}

PlayerStat StatComponent::GetStat(PlayerStatTypes Type)
{
	return m_statData[(int)Type];
}

void StatComponent::SetCurrentStat(double Value, PlayerStatTypes Type)
{
	m_statData[(int)Type].currentValue = Value;
}

void StatComponent::ModifyCurrentStat(double Value, PlayerStatTypes Type)
{
	m_statData[(int)Type].currentValue += Value;
}

void StatComponent::RecalculateStat(PlayerStatTypes Type)
{
	double ratio = (double)m_statData[(int)Type].currentValue / (double)m_statData[(int)Type].maxValue;

	m_statData[(int)Type].maxValue = m_statData[(int)Type].baseValue;

	for (auto mod : m_modifiers[Type])
	{
		if (!mod.isMultiplicative)
			m_statData[(int)Type].maxValue += mod.value;
		else
			m_statData[(int)Type].maxValue *= mod.value;
	}

	m_statData[(int)Type].currentValue = ratio * m_statData[(int)Type].maxValue;
}