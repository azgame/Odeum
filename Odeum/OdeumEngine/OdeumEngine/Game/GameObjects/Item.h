#ifndef ITEM_H
#define ITEM_H

#include "../../pch.h"
#include "../../Engine/Core/GameObject.h"
#include "GameTypes.h"

#include "Observer.h"

class Proc : public Observer
{
public:

	Proc(double Chance) { m_chanceToProc = Chance; }

	void OnNotify(GameObject& GameObject, GameEvent& Event) override
	{
		if (shouldTrigger(Event))
			procCallback(GameObject, Event);
	}

	std::function<bool(GameEvent&)> shouldTrigger;
	std::function<void(GameObject&, GameEvent&)> procCallback;

	double GetProcChance() { return m_chanceToProc; }

private:

	double m_chanceToProc;
};

class ItemEffect
{
public:
	ItemEffect();

	Proc* GetProc() { return proc; }

private:

	bool ShouldTrigger(GameEvent& Action);
	void ChanceToActivate(GameObject& GameObject, GameEvent& Event);

	Proc* proc;
};

class Item
{
public:
	Item(ItemEffect* Effect);

	std::string GetName() { return m_name; }
	std::string GetDescription() { return m_description; }
	UINT GetCost() { return m_cost; }

	ItemEffect* GetItemEffect() { return m_effect; }
	ItemTypes GetItemType() { return m_type; }

private:

	std::string m_name;
	std::string m_description;
	UINT m_cost;
	ItemEffect* m_effect;

	ItemTypes m_type;
};

#endif