#include "APpch.h"
#include "Scene.h"

#include "Components.h"

#include "glm/glm.hpp"

#include "Apostle/Renderer/Renderer2D.h"

namespace Apostle {

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		auto transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto& entity : view)
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent, MeshComponent>();
		for (auto& entity : group)
		{
			auto& [transform, sprite] = m_Registry.get<TransformComponent, SpriteRendererComponent>();

			Renderer2D::DrawQuad(transform, sprite);
		}
#endif
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}


}

