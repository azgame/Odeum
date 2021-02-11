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
	UINT16 id;
	double value;
	bool isUnique;
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
	void Update(float deltaTime) override;
	//static increase is a flat increase ie health = health +10 while modifier will multiply the amount and will stack ie health * 1.2
	void SetHealth(float staticHealthIncrease_, float healthModifier_);
	void SetDefence(float staticDefenceIncrease_, float defenceModifier_);
	void SetSpeed(float staticSpeedIncrease_, float speedModifier_);
	void SetAttack(float staticAttackIncrease_, float attackModifier_,float rockModifier_,float scissorsModifier_,float paperModifier_);
	inline void ResetHealth() { CurrentMaxHealth = baseHealth; };
	inline void ResetDefence() { CurrentMaxDefence = baseDefence; };
	inline void ResetSpeed() { currentSpeed = baseSpeed; };
	inline void ResetAttack() { currentAttack = baseAttack; rockModifier = scissorsModifier = paperModifier = 1; };

	inline float GetRockModifier() { return rockModifier; };
	inline float GetpaperModifier() { return paperModifier; };
	inline float GetScissorsModifier() { return scissorsModifier; };
	GameObject* object;
	inline float GetBaseHealth() { return baseHealth; };
	inline float GetCurrentHealth() { return currentHealth; };
	inline float GetCurrentBaseHealth() { return CurrentMaxHealth; };
	inline float GetBaseDefence() { return baseDefence; };
	inline float GetCurrentDefence() { return currentDefence; };
	inline float GetCurrentBaseDefence() { return CurrentMaxDefence; };
	inline float GetBaseSpeed() { return baseSpeed; };
	inline float GetCurrentSpeed() { return currentSpeed; };
	inline float GetBaseAttack() { return baseAttack; };
	inline float GetCurrentAttack() { return currentAttack; };
	void TakeDamage(float damage_);

	bool HasModifier(UINT16 Id, PlayerStatTypes Type = PlayerStatTypes::None);

	void RegisterLinearModifier(PlayerStatMod Mod);
	void RegisterMulModifier(PlayerStatMod Mod);

	double GetModifier(UINT16 Id, PlayerStatTypes Type = PlayerStatTypes::None);
	PlayerStat GetStat(PlayerStatTypes Type); // returns modified stat

	void SetStat(PlayerStatTypes Type, uint32_t NewValue); // sets stat to given value
	void ModifyStat(PlayerStatTypes Type, uint32_t Value); // modifies stat by given value
	void ResetStat(PlayerStatTypes Type);

	void RecalculateStat();

private:
	//Health
	float baseHealth;
	float CurrentMaxHealth;
	float currentHealth;
	
	//Defence
	float baseDefence;
	float CurrentMaxDefence;
	float currentDefence;
	
	//Speed
	float baseSpeed;
	float currentSpeed;
	
	//Attack
	float baseAttack;
	float rockModifier;
	float paperModifier;
	float scissorsModifier;
	float currentAttack;
	
	std::vector<PlayerStat> m_statData; // default stat data, including current and max values
	std::unordered_map<PlayerStatTypes, std::vector<PlayerStatMod>> m_linModifiers; // modifiers which affect stats linearly, eg. +2 to attack from weapon, +4 to health passive
	std::unordered_map<PlayerStatTypes, std::vector<PlayerStatMod>> m_mulModifiers; // modifiers which affect stats multiplicatively, eg. 10% more attack, 25% damage on certain attack
	// proc effects
};

#endif