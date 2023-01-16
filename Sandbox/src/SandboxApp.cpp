#include <Apostle.h>

class Sandbox : public Apostle::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};


Apostle::Application* Apostle::CreateApplication()
{
	return new Sandbox();
}