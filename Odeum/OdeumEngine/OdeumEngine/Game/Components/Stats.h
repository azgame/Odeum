#ifndef STATS_H
#define STATS_H
#include "../../pch.h"
#include "../../Engine/Core/Component.h"
class Stats : public Component
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
	
};

#endif

