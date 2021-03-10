#include "InventoryComponent.h"
#include "StatComponent.h"
#include "PlayerEffectsComponent.h"

void InventoryComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;

	m_stats = m_gameObject->GetComponent<StatComponent>();
	m_effects = m_gameObject->GetComponent<PlayerEffectsComponent>();

	m_items.resize((int)ItemTypes::Count, nullptr);
}

void InventoryComponent::OnStart()
{
}

bool InventoryComponent::SlotEmpty(ItemTypes Type)
{
	if (Type == ItemTypes::Passive)
	{
		for (int i = (int)ItemTypes::Passive_01; i <= (int)ItemTypes::Passive_03; i++)
			if (m_items[i] == nullptr)
				return true;

		return false;
	}

	return m_items[(int)Type] == nullptr;
}

void InventoryComponent::EquipItem(Item* NewItem)
{
	// Remove stats/effects
	Item* prevItem = m_items[(int)NewItem->GetItemType()];

	if (prevItem != nullptr)
	{
		m_effects->RemoveObserver(prevItem->GetItemEffect()->GetProc());
		m_stats->RemoveModifier(prevItem->GetItemEffect()->GetStat());
	}

	SAFE_DELETE(prevItem);
	
	m_items[(int)NewItem->GetItemType()] = NewItem;

	// Add stats/effects
	m_effects->AddObserver(NewItem->GetItemEffect()->GetProc());
	m_stats->AddModifier(NewItem->GetItemEffect()->GetStat());
}
