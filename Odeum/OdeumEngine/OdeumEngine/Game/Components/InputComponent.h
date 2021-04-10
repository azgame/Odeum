#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../../Engine/Core/Input.h"
#include "../GameObjects/Action.h"

#include <unordered_map>

enum class GameInput
{
	First = KeyCode::J,
	Second = KeyCode::K,
	Alt1 = KeyCode::L,
	Alt2 = KeyCode::I,
	MoveLeft = KeyCode::A,
	MoveRight = KeyCode::D,
	MoveUp = KeyCode::W,
	MoveDown = KeyCode::S,
};

class AnyAction : public Action
{
public:
	AnyAction(GameObject* Actor, std::function<void(GameObject*)> Action);
	void Execute() override;
	GameEventType Type() override { return GameEventType::None; }
private:
	GameObject* actor;
	std::function<void(GameObject*)> action;
};

class InputComponent : public Component
{
public:
	void OnAttach(GameObject* pParent) override { m_gameObject = pParent; };
	void OnDetach() override;
	void OnStart() override {};
	void Update(float deltaTime) override;

	void SetKeyBind(GameInput, std::vector<Action*>);
	bool HasKeyBind(GameInput);

private:

	void ResetToDefaultActions();

	std::unordered_map<GameInput, std::vector<Action*>> inputActions;
	std::unordered_map<GameInput, Action*> defaultActions;
};

#endif