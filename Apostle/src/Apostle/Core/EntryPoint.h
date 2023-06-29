#pragma once

#ifdef AP_PLATFORM_WINDOWS

extern Apostle::Application* Apostle::CreateApplication();

int main(int argc, char** argv)
{
	Apostle::Log::Init();
	
	AP_PROFILE_BEGIN_SESSION("Startup", "ApostleProfile-Startup.json");
	auto app = Apostle::CreateApplication();
	AP_PROFILE_END_SESSION();

	AP_PROFILE_BEGIN_SESSION("Runtime", "ApostleProfile-Runtime.json");
	app->Run();
	AP_PROFILE_END_SESSION();

	AP_PROFILE_BEGIN_SESSION("Shutdown", "ApostleProfile-Shutdown.json");
	delete app;
	AP_PROFILE_END_SESSION();
}

#endif