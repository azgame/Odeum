#include "Action.h"

void Action::Execute()
{
}


MoveAction::MoveAction(Vector2 Move, GameObject* Target)
{
	move = Move;
	target = Target;
}

void MoveAction::Execute()
{
}