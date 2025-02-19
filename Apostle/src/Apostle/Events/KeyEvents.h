#pragma once

#include "Events.h"
#include "Apostle/Core/KeyCodes.h"

namespace Apostle {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeycode() const { return m_Keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(int keycode)
			: m_Keycode(keycode) {}
	
		int m_Keycode;
	};

	class KeyPressedEvent : public KeyEvent
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
			auto it = KeyCodesMap.find(static_cast<KeyCodes>(m_Keycode));
			ss << "Key Pressed: " << it->second << " (Times repeated: " << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			auto it = KeyCodesMap.find(static_cast<KeyCodes>(m_Keycode));
			ss << "Key Released: " << it->second;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			auto it = KeyCodesMap.find(static_cast<KeyCodes>(m_Keycode));
			ss << "Key Typed: " << it->second;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}