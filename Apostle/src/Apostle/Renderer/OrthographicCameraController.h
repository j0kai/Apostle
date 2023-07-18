#pragma once

#include "Apostle/Renderer/OrthographicCamera.h"
#include "Apostle/Core/Timestep.h"

#include "Apostle/Events/ApplicationEvents.h"
#include "Apostle/Events/MouseEvents.h"

namespace Apostle {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController() = default;
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void Resize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		
		// Movement
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraMoveSpeed = 5.0f;
		bool m_IsModified = false;
		
		// Rotation
		bool m_EnableRotation;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 90.0f;
	};
}