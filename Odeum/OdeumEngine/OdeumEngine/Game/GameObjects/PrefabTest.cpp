#include "PrefabTest.h"

PrefabTest::PrefabTest() : GameObject("test", ShapeTypes::CubeShape, Colour(0.2f, 0.9f, 0.7f, 1.0f))
{
	GameObject::Initialize("test");
}

void PrefabTest::Update(float deltaTime)
{

	GameObject::Update(deltaTime);
}
