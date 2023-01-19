#include "Application.h"

#include "Apostle/Events/ApplicationEvents.h"
#include "Apostle/Log.h"

namespace Apostle {

	Application::Application()
	{
	}

	Apostle::Application::~Application()
	{
	}

	void Apostle::Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		AP_TRACE(e);

		while (true);
	}

}