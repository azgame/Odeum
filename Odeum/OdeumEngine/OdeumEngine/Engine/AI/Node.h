#ifndef NODE_H
#define NODE_H

class Node
{
public:
	Node() {};
	 ~Node() {}
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
	
	//typedef NodeStates(*NodeReturn) (<< insert params here >> );
	typedef NodeStates(*NodeReturn());
	virtual NodeStates Evaluate() {};
	

	

protected:
	NodeStates m_nodeState;



};



#endif
