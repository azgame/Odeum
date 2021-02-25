#ifndef ACTION_H
#define ACTION_H

#include "../../pch.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Math/Vector.h"

class Action
{
public:
	virtual void Execute();

protected:
	
};

class MoveAction : public Action
{
public:
	MoveAction(Vector2 Move, GameObject* Target);

	void Execute() override;

private:
	GameObject* target;
	Vector2 move;
};



#endif