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

		m_ActiveScene = CreateRef<Scene>();

		auto square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
		m_SquareEntity = square;

		auto sceneCamera = m_ActiveScene->CreateEntity("Scene Camera");
		sceneCamera.AddComponent<CameraComponent>(glm::ortho(- 16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		m_SceneCamera = sceneCamera;

		auto cc = m_ActiveScene->CreateEntity("Clip-Space Camera");
		cc.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f)).Primary = false;
		m_SecondCamera = cc;
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Apostle::Timestep ts)
	{
		AP_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Apostle::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();
		
		// Update Scene
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
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

		// Statistics Panel
		ImGui::Begin("Statistics");
		
		auto stats = Apostle::Renderer2D::GetStats();
		ImGui::Text("Renderer Statistics: ");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		
		ImGui::End();

		// Inspector Panel
		ImGui::Begin("Inspector");
		
		if (m_SquareEntity)
		{
			auto tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Color: ", glm::value_ptr(squareColor));

			ImGui::Separator();
		}

		if(m_SceneCamera)
		{
			auto tag = m_SceneCamera.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());
			ImGui::DragFloat3("Transform", glm::value_ptr(m_SceneCamera.GetComponent<TransformComponent>().Transform[3]));

			if (ImGui::Checkbox("Render w/ Scene Camera", &m_PrimaryCamera))
			{
				m_SceneCamera.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
				m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
			}
		}
		
		ImGui::End();

		// Scene Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene Viewport");
		
		//Handle Event Blocking
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
 
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.Resize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });
		ImGui::End(); // Scene Viewport END
		ImGui::PopStyleVar();

		ImGui::End(); // Dockspace Demo END
	}

	void EditorLayer::OnEvent(Apostle::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}