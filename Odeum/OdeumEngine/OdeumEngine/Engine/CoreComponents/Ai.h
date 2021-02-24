#ifndef AI_H
#define AI_H
#include "../Core/Component.h"
#include "../AI/ActionNode.h"
#include "../AI/Inverter.h"
#include "../AI/Selector.h"
#include "../AI/Sequence.h"
class Ai :public Component
{
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() {};
	void Update(float deltaTime) override;
	std::vector<Node*> selectorNodes;
	Node* countUpNode;
	Node* countDownNode;
	Node* countSelector;
	NodeStates CountTo200();
	int countTotal;
	NodeStates CountDown();
	bool hitMax;
};


#endif // !