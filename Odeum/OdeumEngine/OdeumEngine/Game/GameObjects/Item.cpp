#include "Item.h"

Item::Item()
{
}

ItemEffect::ItemEffect(Proc* ProcEffect, PlayerStatMod Mod)
{
	proc = ProcEffect; // create this elsewhere
	mod = Mod;
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
