#include "APpch.h"
#include "OrthographicCameraController.h"

#include "Apostle/Core/Input.h"
#include "Apostle/Events/KeyEvents.h"
#include "Apostle/Core/KeyCodes.h"

namespace Apostle {
	
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_EnableRotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		// Movement Controls
		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_A))
		{
			if (m_IsModified)
				m_CameraPosition.x -= (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}

		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_D))
		{
			if (m_IsModified)
				m_CameraPosition.x += (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}

		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_W))
		{
			if (m_IsModified)
				m_CameraPosition.y += (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}

		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_S))
		{
			if (m_IsModified)
				m_CameraPosition.y -= (m_CameraMoveSpeed / 10) * ts;
			else
				m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		// Rotation Controls
		if(m_EnableRotation)
		{
			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_Q))
			{
				if (m_IsModified)
					m_CameraRotation += (m_CameraRotationSpeed / 10) * ts;
				else
					m_CameraRotation += m_CameraRotationSpeed * ts;
			}

			if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_E))
			{
				if (m_IsModified)
					m_CameraRotation -= (m_CameraRotationSpeed / 10) * ts;
				else
					m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		// Movement/Rotation Modifier
		if (Input::IsKeyPressed((int)KeyCodes::AP_KEY_LEFT_SHIFT))
			m_IsModified = true;
		else
			m_IsModified = false;

		m_Camera.SetPosition(m_CameraPosition);

		// m_CameraMoveSpeed = m_ZoomLevel; // Uncomment this for a linear move speed
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AP_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AP_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel); // Set in same way as constructor now that zoom level has changed
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}