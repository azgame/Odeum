#ifndef ITEMREGISTRY_H
#define ITEMREGISTRY_H

// Be able to write proc effects here and attach them to items
// Be able to instantiate an item with effects (stat data) and a proc effect (pre-written function object)

#include "../../pch.h"

#include "Item.h"

namespace ProcRegistry
{
	bool DefaultTrigger(GameEvent& Action);
	void DefaultEffect(GameObject& GameObject, GameEvent& Event);

	bool GoldOnMoveTrigger(GameEvent& Action);
	void GoldOnMoveEffect(GameObject& GameObject, GameEvent& Event);
}

namespace ItemBuilder
{
	Item* GenerateGoldOnMovePassive();
}

#endif