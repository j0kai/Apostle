#include "APpch.h"
#include "Application.h"

#include "Apostle/Events/ApplicationEvents.h"
#include "Apostle/Log.h"

namespace Apostle {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Apostle::Application::~Application()
	{
	}

	void Apostle::Application::Run()
	{
		while (m_IsRunning)
		{
			m_Window->OnUpdate();
		}
	}

}