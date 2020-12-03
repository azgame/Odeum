#ifndef ACTIONNODE_H
#define ACTIONNODE_H
#include "Node.h"
class ActionNode : public Node
{
public:

	
	typedef NodeStates(*ActionNodeDelegate) (void);
	ActionNodeDelegate m_action;
	//protected GameObject ai
	
	 NodeStates Evaluate()override;
	
	 ActionNode(ActionNodeDelegate action);
	
};

#endif