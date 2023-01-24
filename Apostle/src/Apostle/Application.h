#pragma once

#include "Core.h"
#include "Window.h"

namespace Apostle {

	class APOSTLE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
	};

	// To be defined in client
	Application* CreateApplication();
}