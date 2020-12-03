#include "Ai.h"

void Ai::OnAttach(GameObject* parent)
{
	
	ActionNode* countUpNode = new ActionNode(CountTo200());
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