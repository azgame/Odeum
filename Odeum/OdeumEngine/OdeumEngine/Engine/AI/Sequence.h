#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Node.h"
#include <vector>
class Sequence: public Node
{
	Sequence(std::vector<Node> nodes);
	NodeStates Evaluate() override;

protected: std::vector<Node> m_nodes;

};

#endif