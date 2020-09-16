#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "Event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width_, unsigned int height_) : width(width_), height(height_) {}

	// Getters
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << width << ", " << height;
		return ss.str();
	}

	EVENT_TYPE(WindowResize)
	EVENT_CATEGORY(EventCategoryApplication)

private:
	unsigned int width, height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	EVENT_TYPE(WindowClose)
	EVENT_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() = default;

	EVENT_TYPE(AppTick)
	EVENT_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() = default;

	EVENT_TYPE(AppUpdate)
	EVENT_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() = default;

	EVENT_TYPE(AppRender)
	EVENT_CATEGORY(EventCategoryApplication)
};
#endif