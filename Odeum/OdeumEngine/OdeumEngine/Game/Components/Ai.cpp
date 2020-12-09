#include "Ai.h"

void Ai::OnAttach(GameObject* parent)
{
	//Node* countUpNode = new ActionNode([this](auto&&...args)->decltype(auto) {return this->CountTo200(std::forward<decltype(args)>(args)...); });
	 countUpNode = new ActionNode(std::bind(&Ai::CountTo200, this)); // if this doesn't work, use one above
	 countDownNode = new ActionNode(std::bind(&Ai::CountDown, this)); // if this doesn't work, use one above
	selectorNodes.push_back(countUpNode);
	selectorNodes.push_back(countDownNode);
	countTotal = 0;
	 countSelector = new Selector(selectorNodes);
	//countDownNode->Evaluate();
     
}

void Ai::Update(float deltaTime)
{
	countSelector->Evaluate();
	
}
NodeStates Ai::CountTo200()
{
	
	if (countTotal < 2000&&!hitMax)
	{
		countTotal += 1;
		std::cout << "counting up "<< countTotal<< std::endl;
		return NodeStates::RUNNING;
	}
	else
	{
		hitMax = true;
		//std::cout << "counting concluded A " << countTotal << std::endl;
		return NodeStates::FAILURE;
	}
	
}
NodeStates Ai::CountDown()
{
	
	if (countTotal > 0&&hitMax)
	{
		countTotal -= 1;
		std::cout << "counting down " << countTotal << std::endl;
		return NodeStates::RUNNING;
	}
	else
	{
		std::cout << "counting concluded B " << countTotal << std::endl;
		hitMax = false;
		return NodeStates::FAILURE;
	}
}