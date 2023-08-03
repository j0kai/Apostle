#pragma once

#include "entt/entt.hpp"

namespace Apostle {

	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;
	};

}