#ifndef SCENE_H
#define SCENE_H

#include "../../pch.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual bool Initialize() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void UIRender() = 0;
};

#endif