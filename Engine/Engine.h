#pragma once
#include <windows.h>
#include "Win32/WindowContainer.h"
#include "Renderer/DX11.h"
#include "ImGui/ImGuiLayer.h"

class Engine
{
public:
	bool Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight, 
		std::string aWindowTitle = "Snow Engine", const std::string& aWindowClass = "Snow Engine");
	void Begin();
	void End();
	void CleanUp();
	
	bool IsRunning() const { return myIsRunning; };

	static Engine& Get() { return *myInstance; };

private:

	friend class DX11;
	friend class WindowContainer;
	friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	WindowContainer myWindowContainer;
	DX11 myDirectX;
	ImGuiLayer myImGuiLayer;

	bool myIsRunning = true;

	inline static Engine* myInstance = nullptr;
};

