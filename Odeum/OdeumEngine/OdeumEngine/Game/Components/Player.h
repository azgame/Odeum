#ifndef PLAYER_H
#define PLAYER_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../../Engine/Core/OdeumEngine.h"

#include "StatComponent.h"

class Player : public Component {
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) override;

	// getters
	inline std::vector<Key::KeyCode> GetKey() { return playerKeys; };

	// setters
	inline void AddKey(Key::KeyCode key) { playerKeys.push_back(key); };

	void TakeDamage(double Damage);

private:
	std::vector<Key::KeyCode> playerKeys;

	StatComponent* stats;
};

#endif