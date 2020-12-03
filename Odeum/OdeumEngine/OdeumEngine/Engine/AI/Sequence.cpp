#include "Sequence.h"
Sequence::Sequence(std::vector<Node> nodes)
{
	m_nodes = nodes;
}
NodeStates Sequence::Evaluate()
{
    bool anyChildRunning = false;

    for(Node node : m_nodes)
    {
        switch (node.Evaluate())
        {
        case NodeStates::FAILURE:
        {
            m_nodeState = NodeStates::FAILURE;
            return m_nodeState;
        }
        case NodeStates::SUCCESS:
            continue;
        case NodeStates::RUNNING:
        {
            anyChildRunning = true;
            continue;
        }
        default:
        {
            m_nodeState = NodeStates::SUCCESS;
            return m_nodeState;
        }
        }
    }

    m_nodeState = anyChildRunning ? NodeStates::RUNNING : NodeStates::SUCCESS;
    return m_nodeState;
}