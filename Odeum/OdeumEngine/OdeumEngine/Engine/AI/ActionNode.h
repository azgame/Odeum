#ifndef ACTIONNODE_H
#define ACTIONNODE_H
#include "Node.h"
class ActionNode : public Node
{
public:

	typedef NodeStates(*ActionNodeDelegate());
	ActionNodeDelegate m_action;
	//protected GameObject ai
	
	 NodeStates Evaluate()override;
	//delegate NodeStates ActionNodeDelegate();
	 ActionNode(ActionNodeDelegate action);
	
};

#endif