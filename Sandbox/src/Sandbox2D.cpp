#include "Sandbox2D.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	// Square
	m_SquareVA = Apostle::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	Apostle::Ref<Apostle::VertexBuffer> squareVB(Apostle::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{Apostle::ShaderDataType::Float3, "a_Position" },
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Apostle::Ref<Apostle::IndexBuffer> squareIB(Apostle::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Apostle::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Apostle::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Apostle::RenderCommand::Clear();

	Apostle::Renderer::BeginScene(m_CameraController.GetCamera());

	//std::dynamic_pointer_cast<Apostle::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Apostle::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Apostle::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Apostle::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Apostle::Event& e)
{
	m_CameraController.OnEvent(e);
}
