#ifndef SELECTOR_H
#define SELECTOR_H
#include "Node.h"
class Selector : public Node
{
protected:
    List<Node> m_nodes;
public:
    Selector(List<Node> nodes);
    NodeStates Evaluate() override;


};



#endif
