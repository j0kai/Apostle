#pragma once

#include "Events.h"

namespace Apostle {

	class APOSTLE_API KeyEvent : public Event
	{
	public:
		inline int GetKeycode() const { return m_Keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(int keycode)
			: m_Keycode(keycode) {}
	
		int m_Keycode;
	};

	class APOSTLE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Pressed: " << m_Keycode << " (Times repeated: " << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class APOSTLE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Released: " << m_Keycode;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(KeyReleased)
	};

}