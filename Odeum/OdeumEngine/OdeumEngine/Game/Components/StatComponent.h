#ifndef STATCOMPONENT_H
#define STATCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

#include <unordered_map>

enum class PlayerStatTypes
{
	Health,
	Defense,
	Speed,
	Attack,
	RockAttack,
	PaperAttack,
	ScissorsAttack,
	Count,
	None
};

struct PlayerStat
{
	PlayerStatTypes type;
	uint32_t baseValue;
	uint32_t maxValue;
	uint32_t currentValue;
};

struct PlayerStatMod
{
	PlayerStatTypes type;
	uint32_t id;
	double value;
	uint8_t duration;
	bool isUnique;
	bool isMultiplicative;
};

// Testing --------------------------
class StatComponent;

class PlayerStatEvent
{
public:

	void Assign(StatComponent* Owner) { owner = Owner; }
	virtual void Execute() = 0;

	PlayerStatTypes type;
	UINT16 id;

private:

	StatComponent* owner;
};
// ----------------------------------

class StatComponent : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) override;

	bool HasModifier(uint32_t Id, PlayerStatTypes Type = PlayerStatTypes::None);

	PlayerStatMod GetModifier(uint32_t Id, PlayerStatTypes Type = PlayerStatTypes::None);
	PlayerStat GetStat(PlayerStatTypes Type); // returns modified stat

	void AddModifier(PlayerStatMod Mod);

	void SetCurrentStat(double Value, PlayerStatTypes Type);
	void ModifyCurrentStat(double Value, PlayerStatTypes Type);

private:

	void RecalculateStat(PlayerStatTypes Type);
	
	std::vector<PlayerStat> m_statData; // default stat data, including current and max values
	std::unordered_map<PlayerStatTypes, std::vector<PlayerStatMod>> m_modifiers; // modifiers which affect stats
	
	// proc effects
};

#endif