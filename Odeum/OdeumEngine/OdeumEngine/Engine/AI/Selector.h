#ifndef SELECTOR_H
#define SELECTOR_H
#include "Node.h"
#include <vector>
class Selector : public Node
{
protected:
    std::vector<Node*> m_nodes;
    
public:
    Selector(std::vector<Node*> nodes);
    NodeStates Evaluate() override;


};



#endif
