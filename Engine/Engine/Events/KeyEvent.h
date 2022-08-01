#pragma once

#include "Event.h"
#include "Engine/Core/KeyCodes.h"

namespace Snow {

	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return myKeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const KeyCode aKeycode)
			: myKeyCode(aKeycode) {}

		KeyCode myKeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode aKeycode, bool isRepeat = false)
			: KeyEvent(aKeycode), myIsRepeat(isRepeat) {}

		bool IsRepeat() const { return myIsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << myKeyCode << " (repeat = " << myIsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool myIsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode aKeycode)
			: KeyEvent(aKeycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << myKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode aKeycode)
			: KeyEvent(aKeycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << myKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}