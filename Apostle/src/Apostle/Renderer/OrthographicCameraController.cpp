#include "APpch.h"
#include "OrthographicCameraController.h"

#include "Apostle/Core/Input.h"
#include "Apostle/Events/KeyEvents.h"
#include "Apostle/Core/KeyCodes.h"

namespace Apostle {
	
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_EnableRotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		AP_PROFILE_FUNCTION();

		// Movement/Rotation Speed Modifier
		Input::IsKeyPressed((int)KeyCodes::AP_KEY_LEFT_SHIFT) ? m_CameraMoveSpeed = 5.0f : m_CameraMoveSpeed = 2.0f;
		Input::IsKeyPressed((int)KeyCodes::AP_KEY_LEFT_SHIFT) ? m_CameraRotationSpeed = 90.0f : m_CameraRotationSpeed = 40.0f;
		
		// Movement Controls
		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
		}

		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraMoveSpeed * ts;
		}
		
		// Rotation Controls
		if(m_EnableRotation)
		{
			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			/* The following if/else if statement seems useless
			at first but it's useful when calculating the
			difference between two angles */
			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		// m_CameraMoveSpeed = m_ZoomLevel; /* Uncomment this for a linear move speed */
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		AP_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AP_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AP_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		AP_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel); // Set in same way as constructor now that zoom level has changed
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		AP_PROFILE_FUNCTION();

		Resize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}