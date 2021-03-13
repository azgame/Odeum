#include "ItemRegistry.h"

#define T1_HEALTH_MOD_ID 1
#define T2_HEALTH_MOD_ID 2
#define T3_HEALTH_MOD_ID 3
#define T1_DEFENSE_MOD_ID 4
#define T2_DEFENSE_MOD_ID 5
#define T3_DEFENSE_MOD_ID 6
#define T1_ATTACK_MOD_ID 7
#define T2_ATTACK_MOD_ID 8
#define T3_ATTACK_MOD_ID 9
#define T1_SPEED_MOD_ID 10
#define T2_SPEED_MOD_ID 11
#define T3_SPEED_MOD_ID 12


namespace ProcRegistry
{
	double GoldOnMoveChance = 25.0;
}


bool ProcRegistry::DefaultTrigger(GameEvent& Action)
{
	return false;
}

void ProcRegistry::DefaultEffect(GameObject& GameObject, GameEvent& Event)
{
}

bool ProcRegistry::GoldOnMoveTrigger(GameEvent& Action)
{
	if (Action.GetEventType() == GameEventType::GameMove)
		if ((double)(rand() % 100) <= GoldOnMoveChance)
			return true;

	return false;
}

void ProcRegistry::GoldOnMoveEffect(GameObject& GameObject, GameEvent& Event)
{
	// Move
	GameMoveEvent& move = dynamic_cast<GameMoveEvent&>(Event);
	std::cout << "PROC!" << std::endl;
}

Item* ItemBuilder::GenerateGoldOnMovePassive()
{
	Proc* proc = new Proc();
	proc->Trigger = static_cast<bool(*)(GameEvent&)>(&ProcRegistry::GoldOnMoveTrigger);
	proc->Effect = static_cast<void(*)(GameObject&, GameEvent&)>(&ProcRegistry::GoldOnMoveEffect);

	PlayerStatMod mod;
	mod.isMultiplicative = false;
	mod.isUnique = false;
	mod.type = CombatStatTypes::Health;
	mod.value = 2;
	mod.duration = 0;
	mod.id = T1_HEALTH_MOD_ID; // need to generate these mod ids

	ItemEffect* procEffect = new ItemEffect(proc, mod);

	Item* item = new Item();
	item->Initialize("Helmet", "Shit Helm", 1, ItemTypes::Helmet, procEffect);

	return item;
}
