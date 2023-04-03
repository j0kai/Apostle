#include "APpch.h"
#include "OrthographicCameraController.h"

#include "Apostle/Events/KeyEvents.h"
#include "Apostle/KeyCodes.h"

namespace Apostle {
	
#define BIND_EVENT_FUNC(x) std::bind(&OrthographicCameraController::x, this, std::placeholders::_1)

	OrthographicCameraController::OrthographicCameraController(OrthographicCamera& camera)
		: m_Camera(camera)
	{

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		
	}

}