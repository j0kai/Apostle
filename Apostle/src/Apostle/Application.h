#pragma once

#include "Core.h"

namespace Apostle {

	class APOSTLE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();
}