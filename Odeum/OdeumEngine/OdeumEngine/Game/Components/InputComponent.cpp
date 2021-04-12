#include "InputComponent.h"

AnyAction::AnyAction(GameObject* Actor, std::function<void(GameObject*)> Action)
{
	actor = Actor;
	action = Action;
}

void AnyAction::Execute()
{
	action(actor);
}

void InputComponent::OnDetach()
{
	for (auto key : inputActions)
		for (auto action : key.second)
			SAFE_DELETE(action);
}

void InputComponent::Update(float deltaTime)
{
	for (auto key : inputActions)
		if (Input::Get().isKeyPressed((KeyCode)key.first))
			for (auto action : key.second)
				action->Execute();
}

void InputComponent::SetKeyBind(GameInput Key, std::vector<Action*> Actions)
{
	if (HasKeyBind(Key))
	{
		for (auto action : inputActions[Key])
			SAFE_DELETE(action);
	}

	inputActions[Key] = Actions;
}

bool InputComponent::HasKeyBind(GameInput Key)
{
	return inputActions.find(Key) != inputActions.end();
}

void InputComponent::ResetToDefaultActions()
{
	// choose default actions here, or have different actions based on if its going back to map scene or not
}

