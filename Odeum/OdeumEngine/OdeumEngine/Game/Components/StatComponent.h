#ifndef STATCOMPONENT_H
#define STATCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

#include "../GameObjects/GameTypes.h"

#include <unordered_map>

class StatComponent : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) override;
	
	bool HasModifier(uint32_t Id, CombatStatTypes Type = CombatStatTypes::None);

	PlayerStatMod GetModifier(uint32_t Id, CombatStatTypes Type = CombatStatTypes::None);
	PlayerStat GetStat(CombatStatTypes Type); // returns modified stat

	void AddModifier(PlayerStatMod Mod);
	void RemoveModifier(PlayerStatMod Mod);

	void SetCurrentStat(double Value, CombatStatTypes Type);
	void ModifyCurrentStat(double Value, CombatStatTypes Type);

private:

	void RecalculateStat(CombatStatTypes Type);
	void RecalculateAttackStat(CombatStatTypes Type);
	
	std::vector<PlayerStat> m_statData; // default stat data, including current and max values
	std::unordered_map<CombatStatTypes, std::vector<PlayerStatMod>> m_modifiers; // modifiers which affect stats
	
	// proc effects
};

#endif