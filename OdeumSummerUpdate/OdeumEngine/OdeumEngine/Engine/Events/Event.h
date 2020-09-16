#ifndef EVENT_H
#define EVENT_H

#include "../../pch.h"

enum class EventType : uint8_t
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory : uint8_t
{
	None = 0,
	EventCategoryApplication =	0x1,
	EventCategoryInput =		0x2,
	EventCategoryKeyboard =		0x4,
	EventCategoryMouse =		0x8,
	EventCategoryMouseButton =	0x10,
};

// macro so we don't have to retype this in every derived event, thanks cherno
#define EVENT_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
																virtual EventType GetEventType() const override { return GetStaticType(); }

// same here
#define EVENT_CATEGORY(category) virtual uint32_t GetCategoryFlags() const override { return category; }

class Event
{
public:

	virtual ~Event() = default;
	bool handled = false;

	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual uint32_t GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	bool isInCategory(EventCategory category_)
	{
		return GetCategoryFlags() & category_;
	}
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event_) : m_event(event_) {}

	template<typename T, typename F> // F determined by compiler, spooky
	bool Dispatch(const F& func) // signature must match bool func<EventToCheckAgainst>(Event&)
	{
		if (m_event.GetEventType() == T::GetStaticType()) // if our event is equal to the given event type (better be an event)
		{
			m_event.handled = func(static_cast<T&>(m_event)); // dispatch given function on event, and set event to handled (if given function handles and clears event)
			return true;
		}
		return false;
	}

private:
	Event& m_event;
};

#endif