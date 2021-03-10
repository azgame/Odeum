#ifndef ITEMREGISTRY_H
#define ITEMREGISTRY_H

// Be able to write proc effects here and attach them to items
// Be able to instantiate an item with effects (stat data) and a proc effect (pre-written function object)

#include "Item.h"

namespace ItemBuilder
{
	Item* GenerateItem();

}

#endif