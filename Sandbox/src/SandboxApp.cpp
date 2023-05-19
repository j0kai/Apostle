#include <Apostle.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Apostle::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_PerspectiveCamera(45.0f, 16.0f / 9.0f, -0.1f, 1.0f), m_OrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f, -1000.0f, 1000.0f), m_CameraPosition(0.0f)
	{
		// Triangle
		// Vertex Array
		m_VertexArray = Apostle::Ref<Apostle::VertexArray>(Apostle::VertexArray::Create());

		// Vertex Buffer
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
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		Apostle::Ref<Apostle::IndexBuffer> indexBuffer(Apostle::IndexBuffer::Create(indices, sizeof(indices)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_TriangleShader = Apostle::Ref<Apostle::Shader>(Apostle::Shader::Create("assets/shaders/Triangle.glsl"));


		// Square
		// Vertex Array
		m_SquareVA = Apostle::Ref<Apostle::VertexArray>(Apostle::VertexArray::Create());

		// Vertex Buffer
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

		// Index Buffer
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Apostle::Ref<Apostle::IndexBuffer> squareIB(Apostle::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Shaders
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

		//Textures
		auto textureShader = m_ShaderLibrary.LoadShader("assets/shaders/Texture.glsl");

		m_Texture = Apostle::Texture2D::Create("assets/textures/Checkerboard-Grey.png");
		m_ApostleLogoTexture = Apostle::Texture2D::Create("assets/textures/Apostle-Engine-Logo.png");

		std::dynamic_pointer_cast<Apostle::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Apostle::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Apostle::Timestep ts) override
	{
		// Movement Controls
		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_A))
		{
			if (m_IsSpeedModified)
				m_CameraPosition.x -= (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}

		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_D))
		{
			if (m_IsSpeedModified)
				m_CameraPosition.x += (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}

		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_W))
		{
			if (m_IsSpeedModified)
				m_CameraPosition.y += (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}

		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_S))
		{
			if (m_IsSpeedModified)
				m_CameraPosition.y -= (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}

		// Rotation Controls
		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_Q))
		{
			if (m_IsSpeedModified)
				m_CameraRotation += (m_CameraRotationSpeed / 10) * ts;
			else
				m_CameraRotation += m_CameraRotationSpeed * ts;
		}

		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_E))
		{
			if (m_IsSpeedModified)
				m_CameraRotation -= (m_CameraRotationSpeed / 10) * ts;
			else
				m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		// Movement/Rotation
		if (Apostle::Input::IsKeyPressed((int)KeyCodes::AP_KEY_LEFT_SHIFT))
			m_IsSpeedModified = true;
		else
			m_IsSpeedModified = false;
		
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();

		m_OrthoCamera.SetPosition(m_CameraPosition);
		m_OrthoCamera.SetRotation(m_CameraRotation);

		//m_PerspectiveCamera.SetPosition(m_CameraPosition);
		
		//Apostle::Renderer::BeginScene(m_PerspectiveCamera);
		Apostle::Renderer::BeginScene(m_OrthoCamera);

		
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

		// Apostle Logo
		m_ApostleLogoTexture->Bind();
		Apostle::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Apostle::Renderer::Submit(m_TriangleShader, m_VertexArray);

		Apostle::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Apostle::Event& event) override
	{

	}

private:
	Apostle::ShaderLibrary m_ShaderLibrary;
	Apostle::Ref<Apostle::Shader> m_TriangleShader;
	Apostle::Ref<Apostle::VertexArray> m_VertexArray;

	Apostle::Ref<Apostle::Shader> m_FlatColorShader;
	Apostle::Ref<Apostle::VertexArray> m_SquareVA;

	Apostle::Ref<Apostle::Texture2D> m_Texture, m_ApostleLogoTexture;

	Apostle::PerspectiveCamera m_PerspectiveCamera;
	Apostle::OrthographicCamera m_OrthoCamera;
	
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 90.0f;
	bool m_IsSpeedModified = false;


	glm::vec3 m_SquareColor{ 0.2f, 0.3f, 0.8f };
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