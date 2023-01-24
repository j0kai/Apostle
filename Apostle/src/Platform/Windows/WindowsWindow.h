#pragma once

#include "Apostle/Window.h"

#include <GLFW/glfw3.h>

namespace Apostle {

	class APOSTLE_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window Attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool isEnabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}