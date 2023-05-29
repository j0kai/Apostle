#include <Apostle.h>
#include <Apostle/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Apostle::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		// Triangle
		m_TriangleVA = Apostle::Ref<Apostle::VertexArray>(Apostle::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Apostle::Ref<Apostle::VertexBuffer> vertexBuffer(Apostle::VertexBuffer::Create(vertices, sizeof(vertices)));

		Apostle::BufferLayout layout = {

			{ Apostle::ShaderDataType::Float3, "a_Position"},
			{ Apostle::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Apostle::Ref<Apostle::IndexBuffer> indexBuffer(Apostle::IndexBuffer::Create(indices, sizeof(indices)));
		m_TriangleVA->SetIndexBuffer(indexBuffer);

		auto triangleShader = m_ShaderLibrary.LoadShader("assets/shaders/Triangle.glsl");


		// Square
		m_SquareVA =Apostle::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f
		};
		Apostle::Ref<Apostle::VertexBuffer> squareVB(Apostle::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{Apostle::ShaderDataType::Float3, "a_Position" },
			{Apostle::ShaderDataType::Float2, "a_TexCoords" },
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Apostle::Ref<Apostle::IndexBuffer> squareIB(Apostle::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Square Shaders
		std::string flatColorVertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_ModelTransform;

			out vec3 v_Position;			
	
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorFragmentSrc = R"(
			#version 450 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			
			uniform vec3 u_Color;
	
			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";

		m_FlatColorShader = Apostle::Ref<Apostle::Shader>(Apostle::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc));

		// Textures
		auto textureShader = m_ShaderLibrary.LoadShader("assets/shaders/Texture.glsl");

		m_Texture = Apostle::Texture2D::Create("assets/textures/Checkerboard-Grey.png");
		m_ApostleLogoTexture = Apostle::Texture2D::Create("assets/textures/Apostle-Engine-Logo.png");

		std::dynamic_pointer_cast<Apostle::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Apostle::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Apostle::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);
		
		// Render
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();

		Apostle::Renderer::BeginScene(m_CameraController.GetCamera());
				
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::vec3 redColor(0.8f, 0.2f, 0.3f);
		glm::vec3 blueColor(0.2f, 0.3f, 0.8f);

		std::dynamic_pointer_cast<Apostle::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Apostle::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Apostle::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		
		auto textureShader = m_ShaderLibrary.GetShader("Texture");

		// Checkerboard Texture
		m_Texture->Bind();
		Apostle::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		auto triangleShader = m_ShaderLibrary.GetShader("Triangle");
		Apostle::Renderer::Submit(triangleShader, m_TriangleVA);

		// Apostle Logo
		m_ApostleLogoTexture->Bind();
		Apostle::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Apostle::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Apostle::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Apostle::ShaderLibrary m_ShaderLibrary;

	// Triangle
	Apostle::Ref<Apostle::Shader> m_TriangleShader;
	Apostle::Ref<Apostle::VertexArray> m_TriangleVA;

	// Squares
	Apostle::Ref<Apostle::Shader> m_FlatColorShader;
	Apostle::Ref<Apostle::VertexArray> m_SquareVA;

	glm::vec3 m_SquareColor{ 0.2f, 0.3f, 0.8f };

	// Textures
	Apostle::Ref<Apostle::Texture2D> m_Texture, m_ApostleLogoTexture;

	// Camera
	Apostle::OrthographicCameraController m_CameraController;
};

class Sandbox : public Apostle::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};


Apostle::Application* Apostle::CreateApplication()
{
	return new Sandbox();
}