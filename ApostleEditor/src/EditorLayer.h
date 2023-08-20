#pragma once

#include "Apostle.h"

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
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_SceneCamera;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};

}