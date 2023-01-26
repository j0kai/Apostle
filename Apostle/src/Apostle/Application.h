#pragma once

#include "Core.h"

#include "Window.h"
#include "Apostle/LayerStack.h"
#include "Events/Events.h"
#include "Events/ApplicationEvents.h"

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
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;

		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}