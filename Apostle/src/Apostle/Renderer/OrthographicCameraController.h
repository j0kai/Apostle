#pragma once

#include "OrthographicCamera.h"

#include "Apostle/Events/Events.h"

namespace Apostle {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController() = default;
		OrthographicCameraController(OrthographicCamera& camera);
		~OrthographicCameraController() = default;

		void OnEvent(Event& e);

	private:

	private:
		OrthographicCamera m_Camera;
	
	};
}