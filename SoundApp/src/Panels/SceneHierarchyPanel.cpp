#include "SceneHierarchyPanel.h"

//#include <math.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Guacamole/Scene/Components.h"
#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
  #define _CRT_SECURE_NO_WARNINGS
#endif

namespace Guacamole {

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
		ImGui::Begin("SceneHierearchy");
		
		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity(entityID, m_Context.get());
			DrawEntityNode(entity);
		});

		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if(ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if(ImGui::BeginMenu("Create"))
			{
				if(ImGui::MenuItem("Empty"))
					m_SelectionContext = m_Context->CreateEntity("Empty");
				
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if(m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		
		ImGui::End();
	}
	
	void SceneHierarchyPanel::SetSelectedEntity(Entity entity) 
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if(ImGui::IsItemClicked())
			m_SelectionContext = entity;

		// Right-click on entity
		bool entityDeleted = false;
		if(ImGui::BeginPopupContextItem())
		{
			if(ImGui::BeginMenu("Create"))
			{
				if(ImGui::MenuItem("Empty"))
					m_SelectionContext = m_Context->CreateEntity("Empty");
				
				ImGui::EndMenu();
			}
			if(ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGui::TreePop();
		}

		if(entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if(m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}
	
	static void Vec3Control(const std::string& label, glm::vec3 &values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		
		ImGui::PushID(label.c_str());
		
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.3f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();

		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.3f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();

		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.3f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool removable = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if(entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if(ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if(removable && ImGui::BeginPopup("ComponentSettings"))
			{
				if(removable && ImGui::MenuItem("Remove"))
					removeComponent = true;
				
				ImGui::EndPopup();
			}
			
			if(open)
			{
				uiFunction(component);
				
				ImGui::TreePop();
			}

			if(removeComponent)
				entity.RemoveComponent<T>();
		}
	}
	
	void SceneHierarchyPanel::DrawComponents(Entity entity) 
	{
		if(entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if(ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		
		if(ImGui::BeginPopup("AddComponent"))
		{
			if(ImGui::MenuItem("Camera"))
			{
				if(!m_SelectionContext.HasComponent<CameraComponent>())
					m_SelectionContext.AddComponent<CameraComponent>();
				else
					GM_CORE_WARN("This entity already has the Camera Component!");
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("Sprite Renderer"))
			{
				if(!m_SelectionContext.HasComponent<SpriteRendererComponent>())
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
				else
					GM_CORE_WARN("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			Vec3Control("Position", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			Vec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			Vec3Control("Scale", component.Scale);
		}, false);

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };

			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if(ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for(int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if(ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if(isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if(camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
				if(ImGui::DragFloat("Vertical FOV", &perspectiveFOV))
					camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));
				
				float perspectiveNear = camera.GetPerspectiveNearClip();
				if(ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);
				
				float perspectiveFar = camera.GetPerspectiveFarClip();
				if(ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if(camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if(ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);
				
				float orthoNear = camera.GetOrthographicNearClip();
				if(ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);
				
				float orthoFar = camera.GetOrthographicFarClip();
				if(ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);
				
				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImVec4 color = { component.Color.r * 255, component.Color.g * 255, component.Color.b * 255, component.Color.a * 255 };
			
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Color");
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(5, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			ImGui::PushFont(boldFont);
			ImGui::Button("R", buttonSize);
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();

			if(ImGui::DragFloat("##R", &color.x, 0.1f, 0.0f, 255.0f, "%.0f"))
				component.Color.r = color.x / 255;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			ImGui::PushFont(boldFont);
			ImGui::Button("G", buttonSize);
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();

			if(ImGui::DragFloat("##G", &color.y, 0.1f, 0.0f, 255.0f, "%.0f"))
				component.Color.g = color.y / 255;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			ImGui::PushFont(boldFont);
			ImGui::Button("Z", buttonSize);
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();

			if(ImGui::DragFloat("##B", &color.z, 0.1f, 0.0f, 255.0f, "%.0f"))
				component.Color.b = color.z / 255;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushFont(boldFont);
			ImGui::Button("A", buttonSize);
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();

			if(ImGui::DragFloat("##A", &color.w, 0.1f, 0.0f, 255.0f, "%.0f"))
				component.Color.a = color.w / 255;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			if(ImGui::ColorButton("##COLBUTTON", { component.Color.r, component.Color.g, component.Color.b, component.Color.a }))
				ImGui::OpenPopup("picker");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			if (ImGui::BeginPopup("picker"))
        	{
				ImGui::ColorPicker4("##ColPicker", glm::value_ptr(component.Color));
				ImGui::EndPopup();
        	}

			//ImGui::ColorEdit4();
		});
	}

}