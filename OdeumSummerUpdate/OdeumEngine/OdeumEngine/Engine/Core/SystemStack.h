#ifndef SYSTEMSTACK_H
#define SYSTEMSTACK_H

#include "CoreSystem.h"

class SystemStack
{
public:
	SystemStack() = default;
	~SystemStack();

	void Push(CoreSystem* system_);
	void Pop(CoreSystem* system_);

	std::vector<CoreSystem*>::iterator begin() { return m_systems.begin(); }
	std::vector<CoreSystem*>::iterator end() { return m_systems.end(); }
	std::vector<CoreSystem*>::reverse_iterator rbegin() { return m_systems.rbegin(); }
	std::vector<CoreSystem*>::reverse_iterator rend() { return m_systems.rend(); }

	std::vector<CoreSystem*>::const_iterator begin() const { return m_systems.begin(); }
	std::vector<CoreSystem*>::const_iterator end()	const { return m_systems.end(); }
	std::vector<CoreSystem*>::const_reverse_iterator rbegin() const { return m_systems.rbegin(); }
	std::vector<CoreSystem*>::const_reverse_iterator rend() const { return m_systems.rend(); }

private:
	std::vector<CoreSystem*> m_systems;
	uint32_t m_systemIndex = 0;
};

#endif