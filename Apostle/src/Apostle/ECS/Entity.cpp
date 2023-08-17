#include "APpch.h"
#include "Entity.h"

namespace Apostle {

	Entity::Entity(const entt::entity& handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}
