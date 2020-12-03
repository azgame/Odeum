#include "Ai.h"

void Ai::OnAttach(GameObject* parent)
{
	//Node* countUpNode = new ActionNode([this](auto&&...args)->decltype(auto) {return this->CountTo200(std::forward<decltype(args)>(args)...); });
	Node* countUpNode = new ActionNode(std::bind(&Ai::CountTo200, this)); // if this doesn't work, use one above
}

void Ai::Update(float deltaTime)
{

}
NodeStates Ai::CountTo200()
{
	if (countTotal < 200)
	{
		countTotal += 1;
		std::cout << "counting up "<< countTotal<< std::endl;
		return NodeStates::RUNNING;
	}
	else
	{
		std::cout << "counting done " << countTotal << std::endl;
		return NodeStates::FAILURE;
	}
	
}
NodeStates Ai::CountDown()
{
	if (countTotal > 0)
	{
		countTotal -= 1;
		std::cout << "counting down " << countTotal << std::endl;
		return NodeStates::RUNNING;
	}
	else
	{
		std::cout << "counting done " << countTotal << std::endl;
		return NodeStates::FAILURE;
	}
}