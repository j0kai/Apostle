#pragma once

#include "entt/entt.hpp"
#include "Scene.h"

namespace Apostle {

	class Entity
	{
	public:
		Entity() = default;
		Entity(const entt::entity& handle, Scene* scene);
		Entity(const Entity& other) = default;
		
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AP_CORE_ASSERT(!HasComponent<T>(), "Entity already has component of specified type!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			AP_CORE_ASSERT(HasComponent<T>(), "Entity does not have component of specified type!");
			
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			AP_CORE_ASSERT(HasComponent<T>(), "Entity does not have component of specified type!");

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() { return m_EntityHandle != entt::null; }
		operator entt::entity() { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		
		bool operator==(const Entity& other) const 
		{ 
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}