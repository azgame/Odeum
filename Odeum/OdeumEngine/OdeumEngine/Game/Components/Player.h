#ifndef PLAYER_H
#define PLAYER_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../../Engine/Core/OdeumEngine.h"

class StatComponent;

class Player : public Component {
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) override;

	// more complicated way to use keys.
	inline std::vector<Key::KeyCode> GetKey() { return playerKeys; };
	inline void AddKey(Key::KeyCode key) { playerKeys.push_back(key); };

	// more simple way to set up keys.
	inline void SetUp(Key::KeyCode key) { up = key; };
	inline void SetDown(Key::KeyCode key) { down = key; };
	inline void SetRight(Key::KeyCode key) { left = key; };
	inline void SetLeft(Key::KeyCode key) { right = key; };
	inline void SetJump(Key::KeyCode key) { jump = key; };
	inline void SetInteract(Key::KeyCode key) { interact = key; };

	inline Key::KeyCode GetUp() { return up; };
	inline Key::KeyCode GetDown() { return down; };
	inline Key::KeyCode GetRight() { return right; };
	inline Key::KeyCode GetLeft() { return left; };
	inline Key::KeyCode GetJump() { return jump; };
	inline Key::KeyCode GetInteract() { return interact; };

	void TakeDamage(double Damage);
private:
	std::vector<Key::KeyCode> playerKeys;

	Key::KeyCode up, down, left, right, jump, interact;

	StatComponent* stats;
};

#endif