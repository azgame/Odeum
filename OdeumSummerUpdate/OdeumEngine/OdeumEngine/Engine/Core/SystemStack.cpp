#include "SystemStack.h"

SystemStack::~SystemStack()
{
	for (CoreSystem* system : m_systems)
	{
		system->Detach();
		delete system;
	}
}

void SystemStack::Push(CoreSystem* system_)
{
	m_systems.emplace(m_systems.begin() + m_systemIndex, system_);
	m_systemIndex++;
}

void SystemStack::Pop(CoreSystem* system_)
{
	auto it = std::find(m_systems.begin(), m_systems.begin() + m_systemIndex, system_);
	if (it != m_systems.begin() + m_systemIndex)
	{
		system_->Detach();
		m_systems.erase(it);
		m_systemIndex--;
	}
}