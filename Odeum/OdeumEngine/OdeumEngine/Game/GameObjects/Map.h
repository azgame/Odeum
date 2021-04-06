#ifndef MAP_H
#define MAP_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/DataStructures/Graph.h"

class Map
{
public:



private:

	Graph<GameObject*> map = Graph<GameObject*>(100, false);
	std::vector<GameObject*> gameObjects;
};

#endif