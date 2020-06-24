#ifndef CORESYSTEM_H
#define CORESYSTEM_H

#include "../pch.h"
#include "../Events/Event.h"

class CoreSystem
{
public:

	virtual void attach() {}
	virtual void detach() {}
	virtual void update(float deltaTime_) {}
	virtual void uiRender() {}
	virtual void handleEvent(Event& event_) {}
};

#endif