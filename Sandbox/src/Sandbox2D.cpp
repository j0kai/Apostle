#include "Sandbox2D.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <time.h>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Apostle::Texture2D::Create("assets/textures/Checkerboard-Grey.png");
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
	{
		AP_PROFILE_SCOPE("Renderer Prep");
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();
	}

	{
		AP_PROFILE_SCOPE("Renderer Draw");
		Apostle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Apostle::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Apostle::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Apostle::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Apostle::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
		Apostle::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		Apostle::Renderer2D::EndScene();
	}
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
