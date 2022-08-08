#include "SceneHierarchyPanel.h"
#include <Engine/Engine.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>

#include "Engine/Scene/Components.h"
#include "Engine/Scene/ScriptableEntity.h"
#include "GameProject/Scripts.h"

namespace Snow {

	void SceneHierarchyPanel::Init()
	{
		myContext = Engine::GetActiveScene();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };

		if (myContext);
		{
			myContext->myRegistry.each([&](auto entityID)
				{
					Entity entity(entityID, myContext.get());
					if (entity.IsValid() && !entity.HasParent())
					{
						DrawEntityNode(entity);
					}
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				mySelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					myContext->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

				if (payload)
				{
					Entity& entity = *(Entity*)payload->Data;
					myContext->UnparentEntity(entity);
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::End();

			ImGui::Begin("Properties");
			if (myContext->myRegistry.valid(mySelectionContext))
			{
				DrawComponents(mySelectionContext);
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity aEntity)
	{
		auto tag = aEntity.GetComponent<TagComponent>()->name;

		ImGuiTreeNodeFlags flags = ((mySelectionContext == aEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			mySelectionContext = aEntity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		// Drag & Drop
		//------------

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::Text(aEntity.GetComponent<TagComponent>()->name.c_str());
			ImGui::SetDragDropPayload("scene_entity_hierarchy", &aEntity, sizeof(Entity));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

			if (payload)
			{
				Entity& droppedEntity = *(Entity*)payload->Data;
				myContext->ParentEntity(droppedEntity, aEntity);
			}

			ImGui::EndDragDropTarget();
		}

		if (opened)
		{
			for (auto child : aEntity.ChildrenUUIDs())
			{
				Entity e(child, myContext.get());
				if (e.IsValid())
				{
					DrawEntityNode(e);
				}
			}
			
			ImGui::TreePop();
		}
		
		if (entityDeleted)
		{
			myContext->DestroyEntity(aEntity);
			if (mySelectionContext == aEntity)
				mySelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity aEntity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (aEntity.HasComponent<T>())
		{
			auto component = aEntity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			if (typeid(T) != typeid(TagComponent) && typeid(T) != typeid(TransformComponent))
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				aEntity.RemoveComponent<T>();
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!mySelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				mySelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity aEntity)
	{
		if (aEntity.HasComponent<TagComponent>())
		{
			auto& tag = aEntity.GetComponent<TagComponent>()->name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<StaticMeshComponent>("StaticMesh Component");
			DisplayAddComponentEntry<SkeletalMeshComponent>("SkeletalMesh Component");
			DisplayAddComponentEntry<CameraComponent>("Camera Component");
			DisplayAddComponentEntry<NativeScriptComponent>("Native Script Component");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", aEntity, [](auto component)
			{
				DrawVec3Control("Position", component->position);
				glm::vec3 rotation = glm::degrees(component->rotation);
				DrawVec3Control("Rotation", rotation);
				component->rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component->scale, 1.0f);
			});

		DrawComponent<StaticMeshComponent>("Static Mesh", aEntity, [](auto component)
			{

				// #TODO: Change so this doesn't take ref to filepath, instead a copy and update component filepath on button reload click.

				auto& meshFilepath = component->model.myFilePath;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, meshFilepath.c_str(), sizeof(buffer));
				if (ImGui::InputText("StaticMesh", buffer, sizeof(buffer)))
				{
					meshFilepath = std::string(buffer);
				}

				auto& albedoFilepath = component->material.myAlbedo.myFilePath;

				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, albedoFilepath.c_str(), sizeof(buffer));
				if (ImGui::InputText("AlbedoTexture", buffer, sizeof(buffer)))
				{
					albedoFilepath = std::string(buffer);
				}

				float lineWidth = GImGui->Font->FontSize + GImGui->Style.FramePadding.x * 20.0f;
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImVec2 buttonSize = { lineWidth, lineHeight };

				if (ImGui::Button("Reload", buttonSize))
				{
					component->model.LoadModel(meshFilepath.c_str());
					component->material.myAlbedo.LoadTexture(albedoFilepath.c_str());
				}
			});

		DrawComponent<SkeletalMeshComponent>("Skeletal Mesh", aEntity, [](auto component)
			{

				// #TODO: Change so this doesn't take ref to filepath, instead a copy and update component filepath on button reload click.

				auto& meshFilepath = component->animatedModel.myFilePath;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, meshFilepath.c_str(), sizeof(buffer));
				if (ImGui::InputText("SkeletalMesh", buffer, sizeof(buffer)))
				{
					meshFilepath = std::string(buffer);
				}

				auto& animationFilepath = component->animation.myFilePath;

				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, animationFilepath.c_str(), sizeof(buffer));
				if (ImGui::InputText("Animation", buffer, sizeof(buffer)))
				{
					animationFilepath = std::string(buffer);
				}

				auto& albedoFilepath = component->material.myAlbedo.myFilePath;

				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, albedoFilepath.c_str(), sizeof(buffer));
				if (ImGui::InputText("AlbedoTexture", buffer, sizeof(buffer)))
				{
					albedoFilepath = std::string(buffer);
				}

				float lineWidth = GImGui->Font->FontSize + GImGui->Style.FramePadding.x * 20.0f;
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImVec2 buttonSize = { lineWidth, lineHeight };

				if (ImGui::Button("Reload", buttonSize))
				{
					component->animatedModel.LoadModel(meshFilepath.c_str());
					component->material.myAlbedo.LoadTexture(albedoFilepath.c_str());
					component->animation.LoadAnimation(animationFilepath.c_str());
				}
			});

		DrawComponent<CameraComponent>("Camera Component", aEntity, [](auto component)
			{
				bool changesMade = false;
				bool primary = component->camera->GetIsPrimary();
				float fov = component->camera->GetFOV();
				float nearPlane = component->camera->GetNear();
				float farPlane = component->camera->GetFar();
				
				if (ImGui::Checkbox("Primary", &primary))
				{
					changesMade = true;
				}

				if (ImGui::DragFloat("FOV", &fov, 1.f, 30.f, 120.f))
				{
					changesMade = true;
				}

				if (ImGui::DragFloat("Near", &nearPlane, 0.1f, 0.1f, 100.f))
				{
					changesMade = true;
				}

				if (ImGui::DragFloat("Far", &farPlane, 10.f, 100.f, 1000.f))
				{
					changesMade = true;
				}

				if (changesMade)
				{
					component->camera->SetProjectionValues(fov, 16.f/9.f, nearPlane, farPlane);
					component->camera->SetIsPrimary(primary);
				}
			});

		DrawComponent<NativeScriptComponent>("Native Script Component", aEntity, [&](auto component)
			{
				static const char* items[] = { "Example", "Character"};
				static int selectedItem = component->scriptID;

				std::string loadedScriptText = std::string("Loaded: ") + ((component->scriptID >= 0) ? items[component->scriptID] : std::string("None"));

				ImGui::Text(loadedScriptText.c_str());
				ImGui::Combo("Scripts", &selectedItem, items, IM_ARRAYSIZE(items));
				if(ImGui::Button("Load Script"))
				{
					BindNativeScript(aEntity, selectedItem);
				}
			});
	}

	void SceneHierarchyPanel::BindNativeScript(Entity aEntity, int aTypeIndex)
	{
		switch (aTypeIndex)
		{
		case 0:
		{
			auto nsc = aEntity.GetComponent<NativeScriptComponent>();
			if (nsc->Instance)
			{
				nsc->DestroyScript(nsc);
			}
			nsc->Bind<Game::ExampleScript>();
			nsc->scriptID = aTypeIndex;
			break;
		}

		case 1:
		{
			auto nsc = aEntity.GetComponent<NativeScriptComponent>();
			if (nsc->Instance)
			{
				nsc->DestroyScript(nsc);
			}
			nsc->Bind<Game::CharacterScript>();
			nsc->scriptID = aTypeIndex;
			break;
		}

		default:
			break;
		}
	}
}
