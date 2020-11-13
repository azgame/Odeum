#ifndef RENDERER_H
#define RENDERER_H

#include "../../../pch.h"

class Camera;
class Window;

class AbstractRenderer
{
public:
	virtual void Initialize(Window& Window) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Camera& Camera, float deltaTime) = 0;
	virtual void UIRender() = 0;
	virtual void Uninitialize() = 0;
};

#endif