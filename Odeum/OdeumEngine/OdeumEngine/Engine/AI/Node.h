#ifndef NODE_H
#define NODE_H
enum NodeStates
{
	FAILURE,
	SUCCESS,
	RUNNING
};
class Node
{
public:
	Node() {};
	 ~Node() {}
	
	inline NodeStates nodestate()
	{
		 return m_nodeState; 
	};
	
	//typedef NodeStates(*NodeReturn) (<< insert params here >> );
	typedef NodeStates(*NodeReturn());
	virtual NodeStates Evaluate() { return m_nodeState; };
	

	

protected:
	NodeStates m_nodeState;



};



#endif
