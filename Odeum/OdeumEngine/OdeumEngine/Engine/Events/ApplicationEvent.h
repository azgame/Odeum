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
#endif