#pragma once
#include <windows.h>
#include "Win32/WindowContainer.h"
#include "Renderer/Renderer.h"
#include "ImGui/ImGuiLayer.h"
#include "Time/Time.h"
#include "Scene/Scene.h"

class Engine
{
public:
	bool Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight, 
		std::string aWindowTitle = "Snow Engine", const std::string& aWindowClass = "Snow Engine");
	void Begin();
	void End();
	void CleanUp();
	
	void EnableVSync() { myRenderer.myVSyncEnabled = true; };
	void DisableVSync() { myRenderer.myVSyncEnabled = false; };

	bool IsRunning() const { return myIsRunning; };

	Scene& GetCurrentScene() { return CurrentScene; }

	static Engine& Get() { return *myInstance; };

private:
	friend class DX11;
	friend class WindowContainer;
	friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	WindowContainer myWindowContainer;
	Renderer myRenderer;
	ImGuiLayer myImGuiLayer;
	Time myTime;
	
	Scene CurrentScene;

	bool myIsRunning = true;

	inline static Engine* myInstance = nullptr;
};

