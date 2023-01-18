#pragma once

#ifdef AP_PLATFORM_WINDOWS

extern Apostle::Application* Apostle::CreateApplication();

int main(int argc, char** argv)
{
	Apostle::Log::Init();
	AP_CORE_TRACE("Initialised Log!");
	AP_TRACE("Initialised Log!");

	auto app = Apostle::CreateApplication();
	app->Run();
	delete app;
}

#endif