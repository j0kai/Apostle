#include "APpch.h"
#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Components.h"

namespace Apostle {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192503751"; // TODO: Entity UUID will go here eventually.

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // Tag Component

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
				
			out << YAML::EndMap; // Tag Component
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // Transform Component

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // Transform Component
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // Camera Component

			auto& cc = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "Projection Type" << YAML::Value << (int)cc.Camera.GetProjectionType();
			
			out << YAML::Key << "Orthographic Near" << YAML::Value << cc.Camera.GetOrthographicNear();
			out << YAML::Key << "Orthographic Far" << YAML::Value << cc.Camera.GetOrthographicFar();
			out << YAML::Key << "Orthographic Size" << YAML::Value << cc.Camera.GetOrthographicSize();
			
			out << YAML::Key << "Perspective Near" << YAML::Value << cc.Camera.GetPerspectiveNear();
			out << YAML::Key << "Perspective Far" << YAML::Value << cc.Camera.GetPerspectiveFar();
			out << YAML::Key << "Perspective FOV" << YAML::Value << cc.Camera.GetPerspectiveVerticalFOV();
			
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "Fixed Aspect Ratio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap; // Camera Component
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // Sprite Renderer Component

			auto& sr = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << sr.Color;
			
			out << YAML::EndMap; // Sprite Renderer Component
		}
		
		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) 
		{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		// Not implemented
		AP_CORE_ASSERT(false, "Not currently implemented!");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		// Not implemented
		AP_CORE_ASSERT(false, "Not currently implemented!");
		return false;
	}

}