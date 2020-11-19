#ifndef NODE_H
#define NODE_H

class Node
{
public:
	Node() {};
	virtual ~Node() {}
	enum NodeStates
	{
		FAILURE,
		SUCCESS,
		RUNNING
	};
	inline NodeStates nodestate()
	{
		 return m_nodeState; 
	};
	//typedef void  NodeReturn(*NodeStates);
protected:
	NodeStates m_nodeState;



};
















#endif
