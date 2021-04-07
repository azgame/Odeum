#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>
#include <queue>

template<typename T, typename P>
struct PriorityQueue
{
	typedef std::pair<P, T> Node;
	std::priority_queue<Node, std::vector<Node>,
		std::greater<Node>> elements;

	inline bool empty() const
	{
		return elements.empty();
	}

	inline void put(T item, P priority)
	{
		elements.emplace(priority, item);
	}

	inline T get()
	{
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}

	inline int size()
	{
		return elements.size();
	}
};

#endif