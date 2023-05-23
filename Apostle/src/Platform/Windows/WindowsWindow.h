#pragma once

#include "Apostle/Core/Window.h"

#include <GLFW/glfw3.h>
#include "Apostle/Renderer/RenderContext.h"

namespace Apostle {

	class WindowsWindow : public Window
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

		virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		RenderContext* m_Context;

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