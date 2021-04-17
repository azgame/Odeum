#include "Action.h"

#include <stdlib.h>
#include "../Components/PlayerEffectsComponent.h"

void Action::Execute()
{
}

MoveAction::MoveAction(GameObject* Target, Vector2 Move)
{
	move = Move;
	target = Target;
}

void MoveAction::Execute()
{
	//target->GetComponent<PlayerMapUnit>().Move(move);

	std::cout << "Moving target: " << target->Tag() << " to: " << move.GetX() << ", " << move.GetY() << std::endl;

	GameMoveEvent moveEvent = GameMoveEvent(target, move);

	target->GetComponent<PlayerEffectsComponent>()->CheckForProccedEvents(moveEvent);
}