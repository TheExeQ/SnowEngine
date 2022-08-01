#include "ImGuiLayer.h"
#include "Engine/Engine.h"
#include "Engine/Core/DX11.h"
#include <ImGuizmo/ImGuizmo.h>

namespace Snow
{
	void ImGuiLayer::OnAttach()
	{
		HWND hwnd = Engine::GetWindowContainer()->GetWindowHandle();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;

		io.Fonts->AddFontFromFileTTF("../Editor/Assets/Fonts/opensans/OpenSans-Bold.ttf", 18.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("../Editor/Assets/Fonts/opensans/OpenSans-Regular.ttf", 18.f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());

		myLayerActive = true;
	}

	void ImGuiLayer::OnDetach()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		myLayerActive = false;
	}

	void ImGuiLayer::Begin()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}