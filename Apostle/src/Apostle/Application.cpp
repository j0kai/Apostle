#include "APpch.h"
#include "Application.h"

#include "Apostle/Events/ApplicationEvents.h"
#include "Apostle/Log.h"

#include <glad/glad.h>

#include "Input.h"

#include "Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Apostle {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		AP_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		/////////////////////////////////////////////////////////////// 
		// Triangle ///////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////// 
		
		// Vertex Array
		m_VertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());
		
		// Vertex Buffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		BufferLayout layout = {

			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		
		// Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer(IndexBuffer::Create(indices, sizeof(indices)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;	
			out vec4 v_Color;
	
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 450 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader = std::shared_ptr<Shader>(new Shader(vertexSrc, fragmentSrc));
		

		/////////////////////////////////////////////////////////////// 
		// Square ///////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		
		// Vertex Array
		m_SquareVA = std::shared_ptr<VertexArray>(VertexArray::Create());
		
		// Vertex Buffer
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVB(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);
		
		// Index Buffer
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Shaders
		std::string squareVertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			
			out vec3 v_Position;			

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string squareFragmentSrc = R"(
			#version 450 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
				
			void main()
			{
				color = vec4(0.2, 0.8, 0.4, 1.0);
			}

		)";

		m_SquareShader = std::shared_ptr<Shader>(new Shader(squareVertexSrc, squareFragmentSrc));
	}


	Apostle::Application::~Application()
	{
	}

	void Apostle::Application::Run()
	{
		while (m_IsRunning)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_SquareShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));

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
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
}