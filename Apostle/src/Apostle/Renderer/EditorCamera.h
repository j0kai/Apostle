#pragma once

#include "Camera.h"
#include "Apostle\Core\Timestep.h"
#include "Apostle\Events\Events.h"
#include "Apostle\Events\MouseEvents.h"

namespace Apostle {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }
		
		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		const glm::vec3& GetForwardDirection() const { return m_CameraForward; }
		const glm::vec3& GetUpDirection() const { return m_CameraUp; }
		const glm::vec3& GetRightDirection() const { return glm::normalize(glm::cross(m_CameraForward, m_CameraUp)); }
		const glm::vec3& GetPosition() const { return m_Position; }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		bool IsBeingControlled() const { return m_ControlActive; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseTilt(const glm::vec2& delta);
		void MouseZoom(float delta);
	
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 m_CameraForward = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

		glm::vec2 m_InitialMousePosition;
		float m_MouseSensitivity = 8.0f;
		float m_MovementSpeed = 2.0f;
		float m_ZoomSpeed = 10.0f;

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = -90.0f;

		float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;

		bool m_ControlActive = false;
		bool m_RightMouseFirstActive = true;
	};


	constexpr glm::vec3 operator*(const glm::vec3& lhs, float rhs)
	{
		return glm::vec3(
			lhs.x * rhs,
			lhs.y * rhs,
			lhs.z * rhs);
	}

}