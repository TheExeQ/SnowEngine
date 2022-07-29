#include "SceneHierarchyPanel.h"
#include <Engine.h>
#include <imgui/imgui.h>
#include <string>

void SceneHierarchyPanel::Init()
{
	myContext = Engine::GetActiveScene();
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	if (myContext);
	{
		myContext->myRegistry.each([&](auto entityID)
			{
				Entity entity(entityID , myContext);
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			mySelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			//if (ImGui::MenuItem("Create Empty Entity"))
				//myContext->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

	}
	ImGui::End();

	ImGui::Begin("Properties");
	if (&mySelectionContext)
	{
		//DrawComponents(mySelectionContext);
	}

	ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto tag = entity.GetComponent<TagComponent>().name;

	ImGuiTreeNodeFlags flags = ((mySelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		mySelectionContext = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		// Add nodes for children of entity
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		myContext->DestroyEntity(entity);
		if (mySelectionContext == entity)
			mySelectionContext = {};
	}
}
