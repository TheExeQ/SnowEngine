#pragma once
#include <windows.h>
#include "imgui/imgui.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

class ImGuiLayer
{
public:
	ImGuiLayer() = default;
	~ImGuiLayer() = default;

private:
	friend class Engine;

	bool Initialize(HWND hwnd);
	void CleanUp();

	void Begin();
	void End();
};

