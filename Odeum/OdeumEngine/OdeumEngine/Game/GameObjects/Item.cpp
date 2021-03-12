#include "Item.h"

Item::Item()
{
}

ItemEffect::ItemEffect(Proc* ProcEffect)
{
	proc = ProcEffect; // create this elsewhere
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
