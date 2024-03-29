#include "Sandbox2D.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"



Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
	// Triangle
	m_ShaderVA = Apostle::Ref<Apostle::VertexArray>(Apostle::VertexArray::Create());

	float vertices[4 * 3] = {
		-5.0f, -5.0f, 0.0f,
		 5.0f, -5.0f, 0.0f,
		 5.0f,  5.0f, 0.0f,
		-5.0f,  5.0f, 0.0f
	};
	Apostle::Ref<Apostle::VertexBuffer> vertexBuffer(Apostle::VertexBuffer::Create(vertices, sizeof(vertices)));

	Apostle::BufferLayout layout = {
		{ Apostle::ShaderDataType::Float3, "a_Position"}
	};
	vertexBuffer->SetLayout(layout);
	m_ShaderVA->AddVertexBuffer(vertexBuffer);

	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
	Apostle::Ref<Apostle::IndexBuffer> indexBuffer(Apostle::IndexBuffer::Create(indices, sizeof(indices)));
	m_ShaderVA->SetIndexBuffer(indexBuffer);

	auto triangleShader = m_ShaderLibrary.LoadShader("assets/shaders/ShaderArt.glsl");
}

void Sandbox2D::OnAttach()
{
	AP_PROFILE_FUNCTION();
	
	m_CheckerboardTexture = Apostle::Texture2D::Create("assets/textures/Checkerboard-Grey.png");

#ifdef PARTICLE
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.Lifetime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
#endif
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Apostle::Timestep ts)
{
	//using namespace std::literals;
	AP_PROFILE_FUNCTION();
	
	// Update
	{
		AP_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render

	// Reset Renderer Statistics each frame
	Apostle::Renderer2D::ResetStats();

	{
		AP_PROFILE_SCOPE("Renderer Prep");
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();
	}

	{
		AP_PROFILE_SCOPE("Renderer Draw");

		

		/* ----- Shader Art ----- */

		Apostle::Renderer::BeginScene(m_CameraController.GetCamera());

		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedSeconds = currentTime - m_StartTime;
		float time = elapsedSeconds.count();
		auto shaderArtShader = m_ShaderLibrary.GetShader("ShaderArt");
		std::dynamic_pointer_cast<Apostle::OpenGLShader>(shaderArtShader)->Bind();
		std::dynamic_pointer_cast<Apostle::OpenGLShader>(shaderArtShader)->UploadUniformFloat("u_Time", time);
		float width = Apostle::Application::Get().GetWindow().GetWidth();
		float height = Apostle::Application::Get().GetWindow().GetHeight();
		std::dynamic_pointer_cast<Apostle::OpenGLShader>(shaderArtShader)->UploadUniformFloat2("u_Resolution", { width,  height });
		Apostle::Renderer::Submit(shaderArtShader, m_ShaderVA);

		Apostle::Renderer::EndScene();


		/* ----- Basic Scene ----- */
		// static float rotation = 0.0f;
		// rotation += ts * 50.0f;
		//Apostle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//
		//Apostle::Renderer2D::DrawRotatedQuad({ -0.25f, -0.25f, 0.5f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f); // Rotating checkerboard texture
		//Apostle::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f, 0.5f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.2f, 0.8f, 0.7f, 1.0f }); // Rotating teal quad
		//Apostle::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f }); // Static red quad
		//Apostle::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });// Static blue quad
		//Apostle::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f); // Static checkerboard texture

		//for (float y = -5.0f; y < 5.0f; y += 0.5f)
		//{
		//	for (float x = -5.0f; x < 5.0f; x += 0.5f)
		//	{
		//		glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
		//		Apostle::Renderer2D::DrawQuad({ x, y, 0.25f }, { 0.45f, 0.45f }, color);
		//	}
		//}

		//Apostle::Renderer2D::EndScene();
	}
#ifdef PARTICLE
	// Particle System - emits particles from mouse position
	if (Apostle::Input::IsMouseButtonPressed(static_cast<int>(MouseButtonCodes::AP_MOUSE_BUTTON_LEFT)))
	{
		auto [x, y] = Apostle::Input::GetMousePosition();
		auto width = Apostle::Application::Get().GetWindow().GetWidth();
		auto height = Apostle::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	auto stats = Apostle::Renderer2D::GetStats();
	ImGui::Text("Renderer Statistics: ");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void Sandbox2D::OnEvent(Apostle::Event& e)
{
	m_CameraController.OnEvent(e);
}
