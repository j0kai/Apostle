#include "APpch.h"
#include "Application.h"

#include "Apostle/Core/Timestep.h"

#include "Apostle/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Apostle {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		
	{
		AP_PROFILE_FUNCTION();

		AP_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
		m_Window->SetVSync(false);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}


	Apostle::Application::~Application()
	{
		AP_PROFILE_FUNCTION();

	}

	void Apostle::Application::Run()
	{
		AP_PROFILE_FUNCTION();

		while (m_IsRunning)
		{
			AP_PROFILE_SCOPE("Run Loop");

			float time = glfwGetTime();
			Timestep ts = time - m_LastFrameTime;

			m_LastFrameTime = time;

			{
				AP_PROFILE_SCOPE("LayerStack Updates");

				if (!m_IsMinimized)
				{
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}
			}

			
			m_ImGuiLayer->Begin();
			{
				AP_PROFILE_SCOPE("ImGuiLayer Updates");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		AP_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		AP_PROFILE_FUNCTION();

		m_IsRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		AP_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_IsMinimized = true;
			return false;
		}

		m_IsMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}