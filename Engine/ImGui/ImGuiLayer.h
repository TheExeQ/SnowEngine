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

	static ImGuiLayer& Get() { return *myInstance; };
	
private:
	friend class Engine;
	friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool Initialize(HWND hwnd);
	bool CleanUp();

	void Begin();
	void End();

	bool myHasCleanedUp = false;

	inline static ImGuiLayer* myInstance = nullptr;
};

