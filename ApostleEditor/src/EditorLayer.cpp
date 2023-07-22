#include "EditorLayer.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

namespace Apostle {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) // Add true for camera rotation
	{
	}

	void EditorLayer::OnAttach()
	{
		AP_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard-Grey.png");

		FramebufferSpecification framebufferSpecs;
		framebufferSpecs.Width = 1280;
		framebufferSpecs.Height = 720;
		m_Framebuffer = Framebuffer::Create(framebufferSpecs);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Apostle::Timestep ts)
	{
		//using namespace std::literals;
		AP_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render

		// Reset Renderer Statistics each frame
		Apostle::Renderer2D::ResetStats();

		/* --------- Generic Scene - rendering quads and textures ------------ */
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
	}

	void EditorLayer::OnImGuiRender()
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

		// Scene Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene Viewport");
		
		//Handle Event Blocking
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
 
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize)
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
		}
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });
		ImGui::End();

		// Settings Panel
		ImGui::Begin("Settings");
		auto stats = Apostle::Renderer2D::GetStats();
		ImGui::Text("Renderer Statistics: ");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Apostle::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}