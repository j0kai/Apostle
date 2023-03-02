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
		
	}

	void OnEvent(Apostle::Event& event) override
	{
		if (event.GetEventType() == Apostle::EventType::KeyPressed)
		{
			Apostle::KeyPressedEvent& e = (Apostle::KeyPressedEvent&)event;
			if (e.GetKeycode() == (int)KeyCodes::AP_KEY_TAB)
				AP_TRACE("Tab key is pressed (event)!");
			AP_TRACE("{0}", (char)e.GetKeycode());
		}
	}
};

class Sandbox : public Apostle::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};


Apostle::Application* Apostle::CreateApplication()
{
	return new Sandbox();
}