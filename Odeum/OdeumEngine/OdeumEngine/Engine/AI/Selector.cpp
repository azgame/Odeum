#include "Selector.h"
Selector::Selector(std::vector<Node*>  nodes)
{
	m_nodes = nodes;
}
NodeStates Selector::Evaluate()
{
	for  (Node* node : m_nodes)
	{
		switch (node->Evaluate())
		{

		case NodeStates::FAILURE:
			continue;
		case NodeStates::SUCCESS:
		{
			m_nodeState = NodeStates::SUCCESS;
			return m_nodeState;
		}
		case NodeStates::RUNNING:
		{
			m_nodeState = NodeStates::RUNNING;
			return m_nodeState;
		}
		default:
			continue;

		}



	}
}