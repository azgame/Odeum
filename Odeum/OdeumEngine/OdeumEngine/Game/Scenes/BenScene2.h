#ifndef BENSCENE2_H
#define BENSCENE2_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 1
class BenScene2 : public Scene
{
public:
	BenScene2();
	virtual ~BenScene2();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	GameObject* object;
	GameObject* object2;
	float angle;
};

#endif