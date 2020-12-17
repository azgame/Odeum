#include "Inverter.h"
Inverter::Inverter(Node node)
{
	m_node = node;
}
NodeStates Inverter::Evaluate()
{
    switch (m_node.Evaluate())
    {
    case NodeStates::FAILURE:
    {
        m_nodeState = NodeStates::SUCCESS;
        return m_nodeState;
    }
    case NodeStates::SUCCESS:
    {
        m_nodeState = NodeStates::FAILURE;
        return m_nodeState;
    }
    case NodeStates::RUNNING:
    {
        m_nodeState = NodeStates::RUNNING;
        return m_nodeState;
    }
    }
    m_nodeState = NodeStates::SUCCESS;
    return m_nodeState;
}