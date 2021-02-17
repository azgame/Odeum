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
		std::cout << item.value().size();
		m_statData = std::vector<PlayerStat>(item.value().size());

		for (const auto& val : item.value().items())
		{
			PlayerStat newStat;
			newStat.type = (CombatStatTypes)val.value()["EnumID"].get<int>();
			newStat.baseValue = val.value()["BaseValue"].get<int>();

			// can load any saved modifiers here
			auto linMods = val.value()["LinModifiers"];

			for (const auto& lin : linMods.items())
			{
				PlayerStatMod mod;
				// nyi -- load mods
			}

			auto mulMods = val.value()["mulModifiers"];

			for (const auto& mul : mulMods.items())
			{
				PlayerStatMod mod;
				// nyi -- load mods
			}

			newStat.maxValue = newStat.baseValue;
			newStat.currentValue = newStat.maxValue;

			m_statData[val.value()["EnumID"].get<int>()] = newStat;
		}
	}
}

void StatComponent::OnStart()
{
}

void StatComponent::Update(float deltaTime)
{
}

bool StatComponent::HasModifier(uint32_t Id, CombatStatTypes Type)
{
	if (Type != CombatStatTypes::None)
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

PlayerStatMod StatComponent::GetModifier(uint32_t Id, CombatStatTypes Type)
{
	if (Type != CombatStatTypes::None)
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
	noMod.type = CombatStatTypes::None;

	return noMod;
}

PlayerStat StatComponent::GetStat(CombatStatTypes Type)
{
	return m_statData[(int)Type];
}

void StatComponent::SetCurrentStat(double Value, CombatStatTypes Type)
{
	m_statData[(int)Type].currentValue = Value;
}

void StatComponent::ModifyCurrentStat(double Value, CombatStatTypes Type)
{
	m_statData[(int)Type].currentValue += Value;
}

void StatComponent::RecalculateStat(CombatStatTypes Type)
{
	if ((int)Type > 3)
	{
		RecalculateAttackStat(Type);
		return;
	}

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

void StatComponent::RecalculateAttackStat(CombatStatTypes Type)
{
	uint32_t baseAttack = m_statData[(int)CombatStatTypes::Attack].maxValue;

	for (auto mod : m_modifiers[Type])
	{
		if (!mod.isMultiplicative)
			baseAttack += mod.value;
		else
			baseAttack *= mod.value;
	}

	m_statData[(int)Type].currentValue = baseAttack;
	m_statData[(int)Type].maxValue = baseAttack;
}

