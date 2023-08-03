#include "APpch.h"
#include "Scene.h"

#include "glm/glm.hpp"

namespace Apostle {

	Scene::Scene()
	{
		// Temporary code, just playing with the library.
		struct MeshComponent {};

		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: Transform(transform) {}


			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () const { return Transform; }
		};

		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		auto transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto& entity : view)
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		//auto group = m_Registry.group<TransformComponent, MeshComponent>();
	}

	Scene::~Scene()
	{

	}

}

