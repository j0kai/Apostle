#pragma once

#include "Core.h"

#include "Window.h"
#include "Apostle/LayerStack.h"
#include "Events/Events.h"
#include "Events/ApplicationEvents.h"

#include "Apostle/ImGui/ImGuiLayer.h"

namespace Apostle {

	class APOSTLE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window;  }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}