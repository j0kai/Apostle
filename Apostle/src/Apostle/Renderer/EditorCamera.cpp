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
			m_ControlActive = true;
			
			// Handle rotation from mouse inputs
			// Stops camera jumping around when first clicking right mouse.
			if (m_RightMouseFirstActive)
			{
				m_InitialMousePosition = { Input::GetMouseX(), Input::GetMouseY() };
				m_RightMouseFirstActive = false;
			}

			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			MousePan(delta);
			MouseTilt(delta);

			// Handle movement from key inputs
			m_MovementSpeed = Input::IsKeyPressed((int)KeyCodes::AP_KEY_LEFT_SHIFT) ? 4.0f : 2.0f;

			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_W))
			{
				m_Position += GetForwardDirection() * m_MovementSpeed * ts;
			}
			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_S))
			{
				m_Position -= GetForwardDirection() * m_MovementSpeed * ts;
			}
			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_A))
			{
				m_Position -= GetRightDirection() * m_MovementSpeed * ts;
			}
			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_D))
			{
				m_Position += GetRightDirection() * m_MovementSpeed * ts;
			}
		}
		else
		{
			m_ControlActive = false;
			m_RightMouseFirstActive = true;
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AP_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		
		return false;
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation.

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraForward = glm::normalize(direction);

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraForward, m_CameraUp);
	}


	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		m_Yaw += delta.x * m_MouseSensitivity;
	}

	void EditorCamera::MouseTilt(const glm::vec2& delta)
	{
		m_Pitch += -delta.y * m_MouseSensitivity;

		if (m_Pitch > 170.0f)
			m_Pitch = 170.0f;
		if (m_Pitch < -170.0f)
			m_Pitch = -170.0f;

	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_FOV -= delta * m_ZoomSpeed;
		if (m_FOV < 1.0f)
		{
			m_FOV = 1.0f;
		}

		UpdateProjection();
	}

}


