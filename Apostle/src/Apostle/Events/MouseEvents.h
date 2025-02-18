#pragma once

#include "Events.h"
#include "Apostle/Core/MouseButtonCodes.h"

namespace Apostle {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MousePosX(x), m_MousePosY(y)
		{
		}

		inline float GetX() const { return m_MousePosX; }
		inline float GetY() const { return m_MousePosY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse moved to: " << m_MousePosX << ", " << m_MousePosY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MousePosX, m_MousePosY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_ScrollOffsetX(offsetX), m_ScrollOffsetY(offsetY)
		{
		}

		inline float GetOffsetX() const { return m_ScrollOffsetX; }
		inline float GetOffsetY() const { return m_ScrollOffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: Distance Scrolled - X: " << GetOffsetX() << ", Y: " << GetOffsetY();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_ScrollOffsetX, m_ScrollOffsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_MouseButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
	protected:
		MouseButtonEvent(int mouseButton)
			: m_MouseButton(mouseButton){}

		int m_MouseButton;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			auto it = MouseButtonCodesMap.find(static_cast<MouseButtonCodes>(m_MouseButton));
			ss << "Mouse Button Pressed: " << it->second;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			auto it = MouseButtonCodesMap.find(static_cast<MouseButtonCodes>(m_MouseButton));
			ss << "Mouse Button released: " << it->second;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}