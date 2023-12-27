
#include "SceneHierarchyPanel.h"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

namespace Apostle {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		
		m_Context->m_Registry.each([&](auto entityID) 
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Inspector");

		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
		
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, const std::function<void()>& func)
	{
		if (m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str()))
			{
				func();

				ImGui::TreePop();
			}
			else
			{
				func();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{		
		DrawComponent<TagComponent>("Tag", [&]() {

			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		});
		
		DrawComponent<TransformComponent>("Transform", [&]() {
			auto& transform = entity.GetComponent<TransformComponent>().Transform;
			ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
		});

		DrawComponent<CameraComponent>("Camera", [&]() {
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			ImGui::Checkbox("Is Primary", &cameraComponent.Primary);

			if (ImGui::BeginCombo("Projection Type", currentProjectionTypeString))
			{

				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}
				
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			// Show inspector items needed when projection is set to perspective
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
				{
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));
				}

				float perpectiveNear = camera.GetPerspectiveNear();
				if (ImGui::DragFloat("Near Clip", &perpectiveNear))
				{
					camera.SetPerspectiveNear(perpectiveNear);
				}

				float perpectiveFar = camera.GetPerspectiveFar();
				if (ImGui::DragFloat("Far Clip", &perpectiveFar))
				{
					camera.SetPerspectiveFar(perpectiveFar);
				}

			}

			// Show inspector items needed when projection is set to perspective
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Orthographic Size", &orthoSize))
				{
					camera.SetOrthographicSize(orthoSize);
				}

				float orthographicNear = camera.GetOrthographicNear();
				if (ImGui::DragFloat("Near Clip", &orthographicNear))
				{
					camera.SetOrthographicNear(orthographicNear);
				}

				float orthographicFar = camera.GetOrthographicFar();
				if (ImGui::DragFloat("Far Clip", &orthographicFar))
				{
					camera.SetOrthographicFar(orthographicFar);
				}

				ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", [&]() {
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));;
		});
	}

}