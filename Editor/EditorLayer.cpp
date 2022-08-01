#include "EditorLayer.h"
#include <Engine/Engine.h>
#include <Engine/Renderer/MeshFactory.h>
#include <imgui/imgui.h>
#include <iostream>

namespace Snow
{
	void EditorLayer::OnAttach()
	{
		std::cout << "Editor Layer Attached" << std::endl;

		myCube = Engine::GetActiveScene()->CreateEntity("Cube");
		myCube.GetComponent<TransformComponent>()->position = { 2.0f, 0.0f, 0.0f };
		myCube.GetComponent<TransformComponent>()->rotation = { 0.f, 0.0f, 0.0f };
		myCube.AddComponent<StaticMeshComponent>()->model.Initialize(MeshFactory::CreateCube());

		myChest = Engine::GetActiveScene()->CreateEntity("Chest");
		myChest.GetComponent<TransformComponent>()->position = { -2.0f, -0.5f, 0.0f };
		myChest.GetComponent<TransformComponent>()->rotation = { 0.f, glm::radians(-45.0f), 0.0f };
		myChest.GetComponent<TransformComponent>()->scale = { 0.01f, 0.01f, 0.01f };
		myChest.AddComponent<StaticMeshComponent>()->model.Initialize("../Assets/Models/SM/Particle_Chest.fbx");
		myChest.GetComponent<StaticMeshComponent>()->material.SetAlbedo("../Assets/Textures/T_Particle_Chest_C.png");

		myPyramid = Engine::GetActiveScene()->CreateEntity("Pyramid");
		myPyramid.GetComponent<TransformComponent>()->position = { 0.0f, 0.0f, 0.0f };
		myPyramid.GetComponent<TransformComponent>()->rotation = { 0.0f, 0.0f, 0.0f };
		myPyramid.AddComponent<StaticMeshComponent>()->model.Initialize(MeshFactory::CreatePyramid());

		mySceneHierarchyPanel.Init();
		mySceneViewportPanel.Init();
	}

	void EditorLayer::OnImGuiRender()
	{
		RenderDockspace(); // This has to be rendered before windows that should be dockable.
		
		mySceneHierarchyPanel.OnImGuiRender();
		mySceneViewportPanel.OnImGuiRender(mySceneHierarchyPanel.GetSelectedEntity());
	}

	void EditorLayer::OnUpdate()
	{
		
	}

	void EditorLayer::RenderDockspace()
	{
		static bool dockspace_opened = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspace_opened, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		auto oldMinSize = style.WindowMinSize;
		style.WindowMinSize.x = 370.f;
		
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		
		style.WindowMinSize = oldMinSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open...", NULL))
				{
					std::cout << "Open not working atm." << std::endl;
				};
				if (ImGui::MenuItem("Save As...", NULL))
				{
					std::cout << "Save As not working atm." << std::endl;
				};
				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
}
