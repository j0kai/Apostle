#pragma once

#include "Apostle.h"
#include "Panels\SceneHierarchyPanel.h"

#include "Apostle\Renderer\EditorCamera.h"

namespace Apostle {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		// temp
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		// Entities
		Entity m_SquareEntity;
		Entity m_SceneCamera;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = -1; 

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}