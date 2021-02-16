#ifndef BATTLESCENE_H
#define BATTLESCENE_H
#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

#include "../Components/Player.h"


class BattleScene : public Scene
{
public:
	BattleScene();
	virtual ~BattleScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();
	void DamageCalculation(StatComponent* attacker_, StatComponent* defender_, int attackType, int defenceType);
	void DecideFirstTurn(StatComponent* player1_, StatComponent* player2_);
	// 1 rock 2 paper 3 scissors
private:
	StatComponent* player1;
	StatComponent* player2;

	GameObject* player1Object;
	GameObject* player2Object;
	bool player1Turn;
	bool NPCBattle;
	int player1Choice;
	int player2Choice;
	float waitTime;
	std::vector<Key::KeyCode> player1Keys;
	std::vector<Key::KeyCode> player2Keys;
	float extraDamageModifier=1.5f;
	float lessDamageModifier=0.5f;
};


#endif // !BATTLESCENE_H