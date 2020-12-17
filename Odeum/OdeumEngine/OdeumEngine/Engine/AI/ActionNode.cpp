#include "ActionNode.h"


ActionNode::ActionNode(const std::function<NodeStates(void)>& action) : m_action()
{
	m_action = action;
}

NodeStates ActionNode::Evaluate()
{
	switch (m_action())
  {
    case NodeStates::SUCCESS:
    {
        m_nodeState = NodeStates::SUCCESS;
        return m_nodeState;
    }
    case NodeStates::FAILURE:
    {
        m_nodeState = NodeStates::FAILURE;
        return m_nodeState;
    }
    case NodeStates::RUNNING:
    {
        m_nodeState = NodeStates::RUNNING;
        return m_nodeState;
    }
    default:
    {
        m_nodeState = NodeStates::FAILURE;
        return m_nodeState;
    }
  }
}
