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
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}