#ifndef ACTION_H
#define ACTION_H

#include "../../pch.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Math/Vector.h"


#include "Item.h"

#include <functional>

class Action
{
public:
	virtual void Execute() = 0;
	virtual GameEventType Type() = 0;
};

class MoveAction : public Action
{
public:
	MoveAction(GameObject* Target, Vector2 Move);

	void Execute() override;
	GameEventType Type() override { return GameEventType::GameMove; }

private:
	GameObject* target;
	Vector2 move;
};



#endif