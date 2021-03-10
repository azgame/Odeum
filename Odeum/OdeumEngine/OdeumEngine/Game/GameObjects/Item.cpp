#include "Item.h"

Item::Item()
{
}

ItemEffect::ItemEffect(Proc* ProcEffect)
{
	proc = ProcEffect; // create this elsewhere
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
		std::cout << "Proc happened on " << parent->GetName() << std::endl;
	else
		std::cout << "Proc failed on " << parent->GetName() << std::endl;
}

bool ItemEffect::ShouldTrigger(GameEvent& Event)
{
	return Event.GetEventType() == GameEventType::GameMove;
}

void Item::Initialize(std::string Name, std::string Description, UINT Cost, ItemTypes Type, ItemEffect* Effect)
{
	m_name = Name;
	m_description = Description;
	m_cost = Cost;
	m_type = Type;
	m_effect = Effect;
	m_effect->parent = this;
}
