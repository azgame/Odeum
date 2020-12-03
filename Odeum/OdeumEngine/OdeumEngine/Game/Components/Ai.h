#ifndef AI_H
#define AI_H
#include "../Engine/Core/Component.h"
#include "../Engine/AI/ActionNode.h"
#include "../Engine/AI/Inverter.h"
#include "../Engine/AI/Selector.h"
#include "../Engine/AI/Sequence.h"
class Ai :public Component
{
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	std::vector<Node> selectorNodes;
	
	NodeStates CountTo200();
	int countTotal;
	NodeStates CountDown();
	
};


#endif // !