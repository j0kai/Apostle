#include "APpch.h"
#include "PerspectiveCamera.h"

namespace Apostle {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: Camera(glm::perspective(fov, aspectRatio, nearClip, farClip)), m_ViewMatrix(glm::lookAt(glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0)))
	{
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}
	

	void PerspectiveCamera::OnUpdate()
	{

	}

	void PerspectiveCamera::OnEvent(Event& e)
	{

	}

	void PerspectiveCamera::UpdateProjection()
	{

	}

	void PerspectiveCamera::UpdateView()
	{

	}

}