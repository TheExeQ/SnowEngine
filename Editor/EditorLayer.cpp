#include "EditorLayer.h"
#include <Engine/Engine.h>
#include <Engine/Renderer/MeshFactory.h>
#include <Engine/Input/Input.h>
#include <imgui/imgui.h>
#include <iostream>

#include <windows.h>
#include <filesystem>

namespace Snow
{
	void EditorLayer::OnAttach()
	{
		std::cout << "Editor Layer Attached" << std::endl;
		
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
		if (InputManager::Get()->IsKeyPressed(Key::SPACE))
		{
			std::cout << "Space pressed!" << std::endl;
		}
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
				auto originalPath = std::filesystem::current_path();

				OPENFILENAME ofn;
				wchar_t fileName[MAX_PATH] = L"";
				ZeroMemory(&ofn, sizeof(ofn));

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = Engine::GetWindowContainer()->GetWindowHandle();
				ofn.lpstrFilter = L"Scene (*.scene)\0*.scene\0";
				ofn.lpstrFile = fileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"txt";

				std::wstring fileNameStr;
				
				if (ImGui::MenuItem("Open...", NULL))
				{
					if (GetOpenFileName(&ofn))
					{
						std::filesystem::current_path(originalPath);
						fileNameStr = fileName;
						SceneSerializer serializer(Engine::GetActiveScene());
						serializer.Deserialize(std::string(fileNameStr.begin(), fileNameStr.end()).c_str());

						myLastLoadedScene = std::string(fileNameStr.begin(), fileNameStr.end());
					}
				};
				if (ImGui::MenuItem("Save", NULL))
				{
					if (!myLastLoadedScene.empty())
					{
						SceneSerializer serializer(Engine::GetActiveScene());
						serializer.Serialize(myLastLoadedScene.c_str());
					}
				};
				if (ImGui::MenuItem("Save As...", NULL))
				{
					if (GetSaveFileName(&ofn))
					{
						fileNameStr = fileName;
						SceneSerializer serializer(Engine::GetActiveScene());
						serializer.Serialize(std::string(fileNameStr.begin(), fileNameStr.end()).c_str());
						std::filesystem::current_path(originalPath);
						std::cout << std::filesystem::current_path() << std::endl;
					}
				};
				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
}
