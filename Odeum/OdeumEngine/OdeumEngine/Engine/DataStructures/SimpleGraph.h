#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H

#include "../../pch.h"

#include "LinkedList.h"

#include <vector>

template <typename T>
class SimpleGraph
{
public:

	SimpleGraph(int N);
	~SimpleGraph();

	void Push(T Element, std::vector<int> Connections = {});
	void Connect(int First, int Second);
	LinkedList<T>* GetNeighbours(int Index);
	int Size() { return m_graph.size(); }
	int GetIndex(T Element);

private:

	std::vector<LinkedList<T>*> m_graph;

};


template<typename T>
inline SimpleGraph<T>::SimpleGraph(int N)
{
	for (int i = 0; i < N; i++)
	{
		m_graph.push_back(new LinkedList<T>());
	}
}

template<typename T>
inline SimpleGraph<T>::~SimpleGraph()
{
	for (int i = 0; i < Size(); i++)
	{
		SAFE_DELETE(m_graph[i]);
	}

	m_graph.clear();
}

template<typename T>
inline void SimpleGraph<T>::Push(T Element, std::vector<int> Connections)
{
	LinkedList<T>* list = new LinkedList<T>();
	list->push_front(Element);

	for (auto i : Connections)
	{
		list->push_back(m_graph[i]->front());
	}

	m_graph.push_back(list);
}

template<typename T>
inline void SimpleGraph<T>::Connect(int First, int Second)
{
	m_graph[First]->push_back(m_graph[Second]->front());
	m_graph[Second]->push_back(m_graph[First]->front());
}

template<typename T>
inline LinkedList<T>* SimpleGraph<T>::GetNeighbours(int Index)
{
	return m_graph[Index];
}

template<typename T>
inline int SimpleGraph<T>::GetIndex(T Element)
{
	for (int i = 0; i < Size(); i++)
	{
		if (m_graph[i]->front() == Element)
			return i;
	}

	return -1;
}

#endif