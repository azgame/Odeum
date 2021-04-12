#include "Action.h"

#include <stdlib.h>
#include "../Components/PlayerEffectsComponent.h"

#include "../../Engine/CoreComponents/Rigidbody.h"

void Action::Execute()
{
}

MoveAction::MoveAction(GameObject* Target, Vector3 Move)
{
	move = Move;
	target = Target;
}

void MoveAction::Execute()
{
	target->GetComponent<Rigidbody>()->Transpose(Vector4(move));

	GameMoveEvent moveEvent = GameMoveEvent(target, move);
	target->GetComponent<PlayerEffectsComponent>()->CheckForProccedEvents(moveEvent);
}