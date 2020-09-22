#ifndef COMPONENT_H
#define COMPONENT_H

#include "../../pch.h"

class GameObject;

class Component
{
public:
	virtual ~Component() {}
	virtual void OnAttach(GameObject* parent) = 0;
	virtual void OnDetach() = 0;
	virtual void Update(float deltaTime) = 0;
protected:
	GameObject* m_parent;

};

#endif