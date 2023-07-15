#pragma once

#include "Apostle/Renderer/OrthographicCamera.h"
#include "Apostle/Core/Timestep.h"

#include "Apostle/Events/ApplicationEvents.h"
#include "Apostle/Events/MouseEvents.h"

namespace Apostle {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() {return Right - Left; }
		float GetHeight() {return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController() = default;
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
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