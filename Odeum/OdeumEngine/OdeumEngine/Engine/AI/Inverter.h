#ifndef INVERTER_H
#define INVERTER_H
#include "Node.h"
class Inverter :public Node
{
private:
	Node m_node;
	public:
		Inverter(Node node);
		NodeStates Evaluate() override;
};
#endif
