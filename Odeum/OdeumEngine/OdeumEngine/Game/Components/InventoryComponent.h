#ifndef INVENTORYCOMPONENT_H
#define INVENTORYCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../../Engine/Core/OdeumEngine.h"

#include "../GameObjects/Item.h"

#include <unordered_map>

class StatComponent;
class PlayerEffectsComponent;

class InventoryComponent : public Component 
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() override {};
	void OnStart() override;
	void Update(float deltaTime) override {};

	bool SlotEmpty(ItemTypes Slot);
	void EquipItem(Item* Item);

	Item* GetItem(ItemTypes Slot) { return m_items[(int)Slot]; }

private:
	std::vector<Item*> m_items;

	StatComponent* m_stats;				// Reference to one attached to player
	PlayerEffectsComponent* m_effects;	// Reference to one attached to player
};

#endif