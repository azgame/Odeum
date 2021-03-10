#include "Item.h"

Item::Item(ItemEffect* Effect)
{
	m_effect = Effect;
}

ItemEffect::ItemEffect()
{
	proc = new Proc(25.0);
	proc->procCallback = BIND_EVENT_FN(ItemEffect::ChanceToActivate);
	proc->shouldTrigger = BIND_EVENT_FN(ItemEffect::ShouldTrigger);
}

void ItemEffect::ChanceToActivate(GameObject& GameObject, GameEvent& Event)
{
	GameMoveEvent* moveEvent = dynamic_cast<GameMoveEvent*>(&Event);

	double tempChance = proc->GetProcChance();

	if (moveEvent->Distance().GetX() > 1 && moveEvent->Distance().GetY() > 1)
		tempChance = 50.0;

	std::cout << "Chance to activate is " << (int)tempChance << " %" << std::endl;
	if ((double)(rand() % 100) <= tempChance)
		std::cout << "Proc happened!" << std::endl;
	else
		std::cout << "Proc failed." << std::endl;
}

bool ItemEffect::ShouldTrigger(GameEvent& Event)
{
	return Event.GetEventType() == GameEventType::GameMove;
}