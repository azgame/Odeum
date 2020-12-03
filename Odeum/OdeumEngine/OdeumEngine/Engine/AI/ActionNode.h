#ifndef ACTIONNODE_H
#define ACTIONNODE_H
#include "Node.h"

#include <functional>

class ActionNode : public Node
{
public:

	
	//typedef NodeStates (*ActionNodeDelegate)(void);
	//ActionNodeDelegate m_action;
	//protected GameObject ai

	std::function<NodeStates()> m_action;
	
	 NodeStates Evaluate()override;
	
	 ActionNode(const std::function<NodeStates(void)>& action);
	
};

#endif