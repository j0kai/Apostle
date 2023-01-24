#pragma once

#include "APpch.h"
#include "Events/Events.h"

namespace Apostle {

	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(std::string title = "Apostle Engine",
						unsigned int width = 1280,
						unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class APOSTLE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool isEnabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProperties& props = WindowProperties());

	};

}