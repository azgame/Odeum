#include "ItemRegistry.h"

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

	ItemEffect* procEffect = new ItemEffect(proc);

	Item* item = new Item();
	item->Initialize("Helmet", "Shit Helm", 1, ItemTypes::Helmet, procEffect);

	return item;
}
