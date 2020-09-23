#include "PrefabTest.h"
#include "../Components/ComponentTest.h"

PrefabTest::PrefabTest() : GameObject("test", ShapeTypes::CubeShape, Colour(0.6f, 0.3f, 0.7f, 1.0f))
{
	SetPosition(Vector4(-3.0f, 4.0f, -10.f, 1.0f));
	GameObject::Initialize("test");

	AddComponent<ComponentTest>();
}

void PrefabTest::Update(float deltaTime)
{
	for (auto component : m_components)
		component->Update(deltaTime);

	GameObject::Update(deltaTime);
}
