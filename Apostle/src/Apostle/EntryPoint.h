#pragma once

#ifdef AP_PLATFORM_WINDOWS

extern Apostle::Application* Apostle::CreateApplication();

int main(int argc, char** argv)
{
	printf("Apostle Engine");
	auto app = Apostle::CreateApplication();
	app->Run();
	delete app;
}

#endif