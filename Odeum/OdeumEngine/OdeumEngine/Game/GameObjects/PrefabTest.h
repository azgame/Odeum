#ifndef PREFABTEST_H
#define PREFABTEST_H

#include "../../Engine/Core/GameObject.h"

class PrefabTest : public GameObject
{
public:
	PrefabTest();

	void Update(float deltaTime) override;
};

#endif