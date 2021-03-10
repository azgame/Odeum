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

class Item;

class ItemEffect
{
	friend class Item;
public:
	ItemEffect(Proc* ProcEffect);

	Proc* GetProc() { return proc; }
	PlayerStatMod GetStat() { return mod; }

private:

	bool ShouldTrigger(GameEvent& Action);
	void ChanceToActivate(GameObject& GameObject, GameEvent& Event);

	Proc* proc;
	PlayerStatMod mod;

	Item* parent;
};

class Item
{
public:
	Item();
	
	void Initialize(std::string Name, std::string Description, UINT Cost, ItemTypes Type, ItemEffect* Effect);

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