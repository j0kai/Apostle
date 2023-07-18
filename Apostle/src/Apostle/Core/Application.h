#pragma once

#include "Core.h"

#include "Window.h"
#include "Apostle/Core/LayerStack.h"
#include "Apostle/Events/Events.h"
#include "Apostle/Events/ApplicationEvents.h"

#include "Apostle/ImGui/ImGuiLayer.h"

namespace Apostle {

	class Application
	{
	public:
		Application(const std::string& name = "Apostle App");
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window;  }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		
		bool m_IsRunning = true;
		bool m_IsMinimized = false;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}