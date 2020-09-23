#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "Event.h"

#include "../Core/KeyCodes.h"

using namespace Key;

class KeyEvent : public Event
{
public:
	KeyCode GetKeyCode() const { return keyCode; }
	
	EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(const KeyCode keyCode_) : keyCode(keyCode_) {}
	KeyCode keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const KeyCode keyCode_, const uint16_t repeatCount_)
		: KeyEvent(keyCode_), repeatCount(repeatCount_) {}

	uint16_t GetRepeatCount() const { return repeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
		return ss.str();
	}

	EVENT_TYPE(KeyPressed)
private:
	uint16_t repeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const KeyCode keyCode_) : KeyEvent(keyCode_) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << keyCode;
		return ss.str();
	}

	EVENT_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const KeyCode keyCode_) : KeyEvent(keyCode_) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << keyCode;
		return ss.str();
	}

	EVENT_TYPE(KeyTyped)
};

#endif