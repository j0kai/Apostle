#include "APpch.h"
#include "OrthographicCamera.h"

#include "Apostle/Core/Application.h"

namespace Apostle {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, zNear, zFar)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/// <summary>
	/// TODO: These functions (ZoomIn and ZoomOut) should perform a smooth zoom in and out
	/// </summary>
	/*void OrthographicCamera::ZoomIn(float zoomLevel)
	{
		glm::mat4 currentProj = GetProjectionMatrix();
		glm::vec4 zoomMatrix(-zoomLevel, -zoomLevel, -zoomLevel, -zoomLevel);
		

		m_ProjectionMatrix /= zoomMatrix;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::ZoomOut(float zoomLevel)
	{
		glm::mat4 currentProj = GetProjectionMatrix();
		glm::vec4 zoomMatrix(zoomLevel, zoomLevel, zoomLevel, zoomLevel);

		m_ProjectionMatrix *= zoomMatrix;
		RecalculateViewMatrix();
	}*/


	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}