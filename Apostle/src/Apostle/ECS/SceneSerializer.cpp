#include "APpch.h"
#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Components.h"

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}


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
			auto& camera = cc.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();		
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::EndMap; // Camera
			
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

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
		std::ifstream stream(filepath);
		std::stringstream ss;
		ss << stream.rdbuf();

		YAML::Node data = YAML::Load(ss.str());
		if (!data["Scene"])
			return false;

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();
			
				AP_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());
					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());

					cc.Camera.SetPerspectiveNear(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraProps["PerspectiveFar"].as<float>());
					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}


				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		// Not implemented
		AP_CORE_ASSERT(false, "Not currently implemented!");
		return false;
	}

}