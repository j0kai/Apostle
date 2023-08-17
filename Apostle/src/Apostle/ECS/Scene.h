#pragma once

#include "entt/entt.hpp"

#include "Apostle/Core/Timestep.h"

namespace Apostle {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}