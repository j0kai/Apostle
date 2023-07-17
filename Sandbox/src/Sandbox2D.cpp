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

	Apostle::FramebufferSpecification framebufferSpecs;
	framebufferSpecs.Width = 1280;
	framebufferSpecs.Height = 720;
	m_Framebuffer = Apostle::Framebuffer::Create(framebufferSpecs);
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
		m_Framebuffer->Bind();
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();
	}

	{
		AP_PROFILE_SCOPE("Renderer Draw");

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Apostle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		//TODO: Figure out why the framebuffer is rendering in order of draw calls rather than using the Z value
		Apostle::Renderer2D::DrawRotatedQuad({ -0.25f, -0.25f, 0.5f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f); // Rotating checkerboard texture
		Apostle::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f, 0.5f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.2f, 0.8f, 0.7f, 1.0f }); // Rotating teal quad
		Apostle::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f }); // Static red quad
		Apostle::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });// Static blue quad
		Apostle::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f); // Static checkerboard texture

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Apostle::Renderer2D::DrawQuad({ x, y, 0.25f }, { 0.45f, 0.45f }, color);
			}
		}

		Apostle::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
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
	// Note: Set to true for docking
	static bool dockingEnabled = true;

	if(dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit")) Apostle::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollWithMouse);
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });
		ImGui::End();

		ImGui::Begin("Settings");
		auto stats = Apostle::Renderer2D::GetStats();
		ImGui::Text("Renderer Statistics: ");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Apostle::Event& e)
{
	m_CameraController.OnEvent(e);
}
