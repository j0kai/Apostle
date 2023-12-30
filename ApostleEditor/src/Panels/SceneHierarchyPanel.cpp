#include "SceneHierarchyPanel.h"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_internal.h"

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

		// Right-click in blank space
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Inspector");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");
			
			if(ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	/// <summary>
	/// Draws the entities in the Scene Hierarchy Panel
	/// </summary>
	/// <param name="entity"> The entity that you wish to draw. </param>
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		// Right-click on entity
		// TODO: Figure out a way to generate unique IDs for each entity
		// instead of using tag.c_str() - currently doesn't work if multiple 
		// entities have the same tag.
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem(tag.c_str()))
		{
			std::string itemText = "Delete '" + tag + "'";
			if (ImGui::MenuItem(itemText.c_str()))
				entityDeleted = true;
			
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		// Defer entity deletion until end of frame - in case other actions must be performed on it before deletion.
		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			
			if(m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		// Set ImGui to use 2 columns
		ImGui::Columns(2);

		// Label Column
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		// Values Column
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 1, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

		// X Value
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		// Y value
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.8f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.7f, 0.15f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		// Z value
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		// Reset to 1 column
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, const std::function<void()>& func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		
		if (m_SelectionContext.HasComponent<T>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			
			if (ImGui::Button("+", ImVec2{20, 20}))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				func();

				ImGui::TreePop();
			}
			else
			{
				func();
			}
			
			if (removeComponent)
				m_SelectionContext.RemoveComponent<T>();
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
			auto& tc = entity.GetComponent<TransformComponent>();
			DrawVec3Control("Position", tc.Translation);
			
			ImGui::Spacing();

			glm::vec3 rotation = glm::degrees(tc.Rotation);
			DrawVec3Control("Rotation", rotation);
			tc.Rotation = glm::radians(rotation);
			
			ImGui::Spacing();

			DrawVec3Control("Scale", tc.Scale, 1.0f);
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