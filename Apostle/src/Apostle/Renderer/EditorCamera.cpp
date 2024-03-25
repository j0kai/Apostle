#include "APpch.h"
#include "EditorCamera.h"

#include "Apostle\Core\KeyCodes.h"

#include "glm\gtc\matrix_transform.hpp"
#include "Apostle\Core\Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace Apostle {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed((int)MouseButtonCodes::AP_MOUSE_BUTTON_RIGHT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			MousePan(delta);
			MouseTilt(delta);

			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_W))
			{
				m_Position += GetForwardDirection() * 2.0f * ts;
			}

			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_S))
			{
				m_Position -= GetForwardDirection() * 2.0f * ts;
			}

			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_A))
			{
				m_Position -= GetRightDirection() * 2.0f * ts;
			}
			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_D))
			{
				m_Position += GetRightDirection() * 2.0f * ts;
			}
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AP_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation.
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraForward, m_CameraUp);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		float xSpeed = PanSpeed();
		m_Yaw += delta.x * xSpeed * m_Distance;
	}

	void EditorCamera::MouseTilt(const glm::vec2& delta)
	{
		float ySpeed = TiltSpeed();
		float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		m_Pitch += yawSign * delta.y * ySpeed;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_Distance = 1.0f;
		}
	}

	float EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		
		return xFactor;
	}

	float EditorCamera::TiltSpeed() const
	{
		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return yFactor;
	}

	float EditorCamera::MoveSpeed() const
	{
		return 2.0f;
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::max(speed, 100.0f); // max speed = 100.0f
		return speed;
	}

}


