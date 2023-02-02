#pragma once

#include "Apostle/Core.h"
#include "Apostle/Layer.h"
#include "Apostle/Events/ApplicationEvents.h"

namespace Apostle {

	class APOSTLE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
		
		void BlockEvents(bool block) { m_EventsBlocked = block; }
	private:
		bool OnImGuiWindowCloseEvent(Event& e);
	private:
		bool m_EventsBlocked = true;
		float m_Time = 0.0f;
	};

}