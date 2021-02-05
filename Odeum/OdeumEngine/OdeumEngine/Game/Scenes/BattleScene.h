#ifndef BATTLESCENE_H
#define BATTLESCENE_H
#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../Components/Stats.h"
class BattleScene : public Scene
{
	BattleScene(Stats p1_,Stats p2_);
	virtual ~BattleScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();
	void DamageCalculation(Stats* attacker_, Stats* defender_, int attackType, int defenceType);
	// 1 rock 2 paper 3 scissors
private:
	Stats player1;
	Stats player2;
	float extraDamageModifier=1.5f;
	float lessDamageModifier=0.5f;
};


#endif // !BATTLESCENE_H