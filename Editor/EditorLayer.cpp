#include "EditorLayer.h"
#include <Engine/Engine.h>
#include <Engine/Input/Input.h>
#include <Engine/Debug/Log.h>
#include "Engine/Scene/Components.h"

#include <imgui/imgui.h>
#include <windows.h>
#include <filesystem>
#include <sstream>

namespace Snow
{
	void EditorLayer::OnAttach()
	{
		LOG_INFO("Editor Layer Attached");
		
		myEditorCameraEntity = Engine::GetEditorScene()->CreateEntity("EditorCamera", Engine::GetEditorScene());
		myEditorCameraEntity.AddComponent<CameraComponent>();

		Engine::SetEditorCamera(myEditorCameraEntity);
		
		myEditorCamera->SetIsPrimary(true);
		
		mySceneHierarchyPanel.Init();
		mySceneViewportPanel.Init();

		myPlayIcon->LoadTexture("../Editor/Assets/Icons/PlayButton.png");
		myStopIcon->LoadTexture("../Editor/Assets/Icons/StopButton.png");
	}

	void EditorLayer::OnImGuiRender()
	{
		RenderDockspace(); // This has to be rendered before windows that should be dockable.
		UI_Toolbar();
		
		mySceneHierarchyPanel.OnImGuiRender();
		mySceneViewportPanel.OnImGuiRender(mySceneHierarchyPanel.GetSelectedEntity());
	}

	void EditorLayer::OnUpdate()
	{
		LOG_INFO(std::string("FPS: " + std::to_string(Time::GetFPS())));
		auto* transformComp = myEditorCameraEntity.GetComponent<TransformComponent>();
		myEditorCamera->UpdateMovement(transformComp->position, transformComp->rotation);
		myEditorCameraEntity.GetComponent<CameraComponent>()->camera = myEditorCamera;
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
				
				if (ImGui::MenuItem("New", NULL))
				{
					Engine::GetActiveScene()->ClearScene();
				};
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
					}
				};
				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		auto sceneState = Engine::GetActiveScene()->GetSceneState();
		bool toolbarEnabled = true;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
		{
			tintColor.w = 0.5f;
		}

		float size = ImGui::GetWindowHeight() - 4.0f;
		{
			Ref<Texture> icon = (sceneState == SceneState::Edit || sceneState == SceneState::Simulate) ? myPlayIcon : myStopIcon;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (ImGui::ImageButton(icon->GetTextureView().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (sceneState == SceneState::Edit || sceneState == SceneState::Simulate)
				{
					Engine::GetActiveScene()->OnRuntimeStart();
					//OnScenePlay();
				}
				else if (sceneState == SceneState::Play)
				{
					Engine::GetActiveScene()->OnRuntimeStop();
					//OnSceneStop();
				}
			}
		}
		/* SIMULATE BUTTON
		ImGui::SameLine();
		{
			Ref<Texture> icon = (sceneState == SceneState::Edit || sceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;		//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (ImGui::ImageButton(icon->GetTextureView().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (sceneState == SceneState::Edit || sceneState == SceneState::Play)
				{
					//OnSceneSimulate();
				}
				else if (sceneState == SceneState::Simulate)
				{
					//OnSceneStop();
				}
			}
		}
		*/
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}
