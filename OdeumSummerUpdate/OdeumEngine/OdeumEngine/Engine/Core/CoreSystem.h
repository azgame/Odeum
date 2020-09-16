#ifndef CORESYSTEM_H
#define CORESYSTEM_H

#include "../../pch.h"
#include "../Events/Event.h"

#include <queue>

class CoreSystem
{
public:

	CoreSystem(const std::string& name_ = "System") : m_debugName(name_) {}
	virtual ~CoreSystem() = default;

	virtual void Attach() {}
	virtual void Detach() {}
	virtual void Update(float deltaTime_) {}
	virtual void UIRender() {}
	virtual void HandleEvent(Event& event_) {}

	const std::string& Name() const { return m_debugName; }

protected:
	std::string m_debugName;

	std::vector<Event&> m_eventQueue;
	uint16_t m_bufferHead;
	uint16_t m_bufferTail;
	uint16_t m_eventFrameLimit;
};

#endif