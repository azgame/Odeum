#ifndef CORESYSTEM_H
#define CORESYSTEM_H

#include "../../pch.h"
#include "../Events/Event.h"

#include "../DataStructures/RingBuffer.h"

class CoreSystem
{
public:

	CoreSystem(const std::string& name_ = "System") : m_debugName(name_), m_eventQueue(16) {}
	virtual ~CoreSystem() = default;

	virtual void Attach() {}
	virtual void Detach() {}
	virtual void Update(float deltaTime_) {}
	virtual void UIRender() {}
	virtual void HandleEvent(Event& event_) {}

	const std::string& Name() const { return m_debugName; }

protected:
	std::string m_debugName;

	RingBuffer<Event*> m_eventQueue;
};

#endif