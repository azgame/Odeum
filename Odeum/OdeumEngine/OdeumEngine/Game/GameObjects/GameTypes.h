#ifndef GAMETYPES_H
#define GAMETYPES_H

enum class CombatStatTypes
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

enum class ItemTypes
{
	Weapon,
	Helmet,
	Chest,
	Passive_01, // dumb way to do this
	Passive_02,
	Passive_03,
	Count,
	Passive,
	None
};

struct PlayerStat
{
	CombatStatTypes type;
	uint32_t baseValue;
	uint32_t maxValue;
	uint32_t currentValue;
};

struct PlayerStatMod
{
	CombatStatTypes type;
	uint32_t id;
	double value;
	uint8_t duration;
	bool isUnique;
	bool isMultiplicative;
};

#endif