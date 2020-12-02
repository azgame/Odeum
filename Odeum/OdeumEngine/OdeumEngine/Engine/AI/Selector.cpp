#include "Selector.h"
Selector::Selector(List<Node>  nodes)
{
	m_nodes = nodes;
}
NodeStates Selector::Evaluate()
{
	for each (Node node in m_nodes)
	{
		switch (node.Evaluate())
		{

		case NodeStates.FAILURE:
			continue;
		case NodeStates.SUCCESS:
			m_nodeState = NodeStates.SUCCESS;
			return m_nodestate;
		case NodeStates.RUNNING:
			m_nodeState = NodeStates.RUNNING;
			return m_nodestate;


		}



	}
}