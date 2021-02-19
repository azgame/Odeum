#ifndef DiegoGuessScene_H
#define DiegoGuessScene_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 9
class GuessScene : public Scene
{
public:
	GuessScene();
	virtual ~GuessScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:

	std::vector<GameObject*> gameObjects;



	std::vector<GameObject*> playerObjects;

	std::vector<bool> PLayerPushedButtonForDoorChoice;

};

#endif
