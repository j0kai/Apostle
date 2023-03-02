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
		virtual void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		
	private:
		bool m_BlockEvents = true;
	};

}