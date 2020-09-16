#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "Event.h"

// still have to add input! (MouseCode)
class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x_, const float y_) : mouseX(x_), mouseY(y_) {}

	// Getters
	float GetX() const { return mouseX; }
	float GetY() const { return mouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
		return ss.str();
	}

	EVENT_TYPE(MouseMoved)
	EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float mouseX, mouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset_, const float yOffset_) : xOffset(xOffset_), yOffset(yOffset_) {}

	// Getters
	float GetXOffset() const { return xOffset; }
	float GetYOffset() const { return yOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_TYPE(MouseScrolled)
	EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float xOffset, yOffset;
};

class MouseButtonEvent : public Event
{
public:
	// Getter
	MouseCode GetMouseButton() const { return button; }

	EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	MouseButtonEvent(const MouseCode button_) : button(button_) {}
	MouseCode button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseCode button_) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << button;
		return ss.str();
	}

	EVENT_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
	MouseButtonReleasedEvent(const MouseCode button_) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << button;
		return ss.str();
	}

	EVENT_TYPE(MouseButtonReleased)
};
#endif