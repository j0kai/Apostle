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
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		
		m_Context->m_Registry.each([&](auto entityID) 
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
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

		ImGui::Begin("Details");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			ImGui::Dummy(ImVec2{ 0.0f, 10.0f });

			// Calculate position of Add Component button using text size.
			std::string label = "Add Component";
			ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
			ImVec2 padding { 75.0f, GImGui->Style.FramePadding.y };
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
			ImVec2 size = ImGui::CalcItemSize(ImVec2{0, 0}, textSize.x + GImGui->Style.FramePadding.x * 2.0f, textSize.y + GImGui->Style.FramePadding.y * 2.0f);
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5f - (size.x * 0.5f));
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}
			ImGui::PopStyleVar();
			
			// Handles what is shown in the popup menu.
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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		// Right-click on entity
		// TODO: Figure out a way to generate unique IDs for each entity
		// instead of using tag.c_str() - currently doesn't work if multiple 
		// entities have the same name.
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

	/* Functions to draw ImGui widgets w/ the label on the left using tables */
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushID(label.c_str());

		std::string name = "##Vec3Control" + label;
		if (ImGui::BeginTable(name.c_str(), 2, ImGuiTableFlags_SizingStretchSame))
		{
			ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch, 1);
			ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch, 2);

			for (int row = 0; row < 1; ++row)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < 2; ++col)
				{
					ImGui::TableSetColumnIndex(col);

					if (col == 0)
					{
						ImGui::AlignTextToFramePadding();
						ImGui::Text(label.c_str());
					}
					else
					{
						ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 1, 0 });

						float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
						ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

						// X Value
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.35f, 1.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
						ImGui::PushFont(io.Fonts->Fonts[0]);
						if (ImGui::Button("X", buttonSize))
						{
							values.x = resetValue;
						}
						ImGui::PopFont();
						ImGui::PopStyleColor(3);
						ImGui::SameLine();
						ImGui::DragFloat("##X", &values.x, 0.1f);
						ImGui::PopItemWidth();

						ImGui::SameLine();

						// Y value
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.15f, 1.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.8f, 0.35f, 1.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.7f, 0.15f, 1.0f });
						ImGui::PushFont(io.Fonts->Fonts[0]);
						if (ImGui::Button("Y", buttonSize))
						{
							values.y = resetValue;
						}
						ImGui::PopFont();
						ImGui::PopStyleColor(3);
						ImGui::SameLine();
						ImGui::DragFloat("##Y", &values.y, 0.1f);
						ImGui::PopItemWidth();

						ImGui::SameLine();

						// Z value
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.9f, 1.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
						ImGui::PushFont(io.Fonts->Fonts[0]);
						if (ImGui::Button("Z", buttonSize))
						{
							values.z = resetValue;
						}
						ImGui::PopFont();
						ImGui::PopStyleColor(3);
						ImGui::SameLine();
						ImGui::DragFloat("##Z", &values.z, 0.1f);
						ImGui::PopItemWidth();

						ImGui::PopStyleVar();
					}
				}
			}

			ImGui::EndTable();
		}

		ImGui::PopID();
	}

	static void DrawDragFloat(const std::string& label, float& value, bool isChild = false)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushID(label.c_str());

		if (isChild)
			ImGui::Indent();

		std::string name = "##DragFloat" + label;
		if (ImGui::BeginTable(name.c_str(), 2))
		{
			ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch);

			for (int row = 0; row < 1; ++row)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < 2; ++col)
				{
					ImGui::TableSetColumnIndex(col);

					if (col == 0)
					{
						ImGui::AlignTextToFramePadding();
						ImGui::Text(label.c_str());
					}
					else
					{
						ImGui::DragFloat(name.c_str(), &value);
					}
				}
			}

			ImGui::EndTable();
		}

		if (isChild)
			ImGui::Unindent();

		ImGui::PopID();
	}

	static void DrawCheckbox(const std::string& label, bool& value, bool isChild = false)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushID(label.c_str());

		if (isChild)
			ImGui::Indent();

		std::string name = "##Checkbox" + label;
		if (ImGui::BeginTable(name.c_str(), 2))
		{
			ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch);

			for (int row = 0; row < 1; ++row)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < 2; ++col)
				{
					ImGui::TableSetColumnIndex(col);

					if (col == 0)
					{
						ImGui::AlignTextToFramePadding();
						ImGui::Text(label.c_str());
					}
					else
					{
						ImGui::Checkbox(name.c_str(), &value);
					}
				}
			}

			ImGui::EndTable();
		}

		if (isChild)
			ImGui::Unindent();

		ImGui::PopID();
	}

	static void DrawColorEdit(const std::string& label, glm::vec4& color, bool isChild = false)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushID(label.c_str());

		if (isChild)
			ImGui::Indent();

		std::string name = "##ColorEdit" + label;
		if (ImGui::BeginTable(name.c_str(), 2, ImGuiTableFlags_SizingStretchSame))
		{
			ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch, 1);
			ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch, 4);

			for (int row = 0; row < 1; ++row)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < 2; ++col)
				{
					ImGui::TableSetColumnIndex(col);

					if (col == 0)
					{
						ImGui::AlignTextToFramePadding();
						ImGui::Text(label.c_str());
					}
					else
					{
						ImGui::ColorEdit4(label.c_str(), glm::value_ptr(color), ImGuiColorEditFlags_NoLabel);
					}
				}
			}

			ImGui::EndTable();
		}

		if (isChild)
			ImGui::Unindent();

		ImGui::PopID();
	}

	template<typename T>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, const std::function<void()>& func, bool isRemovable)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;
		auto contentAvailRegion = ImGui::GetContentRegionAvail();

		if (m_SelectionContext.HasComponent<T>())
		{	
			if (typeid(T) == typeid(TagComponent))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 3 });
				ImGui::Text("Name");
				ImGui::SameLine(60.0f);
				ImGui::PopStyleVar();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 2 });
				func();
				ImGui::PopStyleVar();
				
				ImGui::Spacing();

				return;
			}
			
			
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentAvailRegion.x - lineHeight * 0.5f);
			
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (isRemovable)
			{
				if (ImGui::BeginPopupContextWindow() || ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						removeComponent = true;
					}

					ImGui::EndPopup();
				}
			}
			
			if (open)
			{
				func();

				ImGui::TreePop();
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
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll))
			{
				tag = std::string(buffer);
			}
		}, false);
		
		DrawComponent<TransformComponent>("Transform", [&]() {
			auto& tc = entity.GetComponent<TransformComponent>();
			DrawVec3Control("Position", tc.Translation);
			
			ImGui::Spacing();

			glm::vec3 rotation = glm::degrees(tc.Rotation);
			DrawVec3Control("Rotation", rotation);
			tc.Rotation = glm::radians(rotation);
			
			ImGui::Spacing();

			DrawVec3Control("Scale", tc.Scale, 1.0f);

			ImGui::Spacing();
		}, false);

		DrawComponent<CameraComponent>("Camera", [&]() {
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			DrawCheckbox("Is Primary", cameraComponent.Primary);
			
			if (ImGui::BeginTable("Projection Type", 2, ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch);
				
				for (int row = 0; row < 1; ++row)
				{
					ImGui::TableNextRow();
					for (int col = 0; col < 2; ++col)
					{
						ImGui::TableSetColumnIndex(col);
						
						if (col == 0)
						{
							ImGui::AlignTextToFramePadding();
							ImGui::Text("Projection Type");
						}
						else
						{
							if (ImGui::BeginCombo("##Projection Type", currentProjectionTypeString))
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
						}
					}
				}
				
				ImGui::EndTable();
			}

			// Show inspector items needed when projection is set to perspective
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
				DrawDragFloat("Vertical FOV", verticalFOV, true);
				camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));
				
				float perpectiveNear = camera.GetPerspectiveNear();
				DrawDragFloat("Near Clip", perpectiveNear, true);
				camera.SetPerspectiveNear(perpectiveNear);

				float perpectiveFar = camera.GetPerspectiveFar();
				DrawDragFloat("Far Clip", perpectiveFar, true);
				camera.SetPerspectiveFar(perpectiveFar);
			}

			// Show inspector items needed when projection is set to perspective
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				DrawDragFloat("Size", orthoSize, true);
				camera.SetOrthographicSize(orthoSize);

				float orthographicNear = camera.GetOrthographicNear();
				DrawDragFloat("Near Clip", orthographicNear, true);
				camera.SetOrthographicNear(orthographicNear);

				float orthographicFar = camera.GetOrthographicFar();
				DrawDragFloat("Far Clip", orthographicFar, true);
				camera.SetOrthographicFar(orthographicFar);

				DrawCheckbox("Fixed Aspect Ratio", cameraComponent.FixedAspectRatio, true);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", [&]() {
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			DrawColorEdit("Color", src.Color);
		});
	}


}