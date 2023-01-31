#include <Apostle.h>

class ExampleLayer : public Apostle::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{
	}

	void OnUpdate() override
	{
		AP_INFO("ExampleLayer::Update");
	}

	void OnEvent(Apostle::Event& event) override
	{
		AP_TRACE("{0}", event);
	}
};

class Sandbox : public Apostle::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Apostle::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};


Apostle::Application* Apostle::CreateApplication()
{
	return new Sandbox();
}