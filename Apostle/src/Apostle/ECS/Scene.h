#pragma once

#include "entt/entt.hpp"

#include "Apostle/Core/Timestep.h"

namespace Apostle {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(Timestep ts);

		entt::registry& Reg() { return m_Registry; }
	private:
		entt::registry m_Registry;
	};

}