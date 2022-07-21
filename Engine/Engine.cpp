#include "Engine.h"
#include <iostream>

bool Engine::Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight, std::string aWindowTitle, const std::string& aWindowClass)
{
	auto result = myWindowContainer.Initialize(hInstance, aWidth, aHeight, aWindowTitle, aWindowClass);
	if (!result)
	{
		std::cout << "Failed to initialize Win32" << std::endl;
		std::cin.get();
		return false;
	}

	HWND handle = myWindowContainer.GetWindowHandle();

	result = myDirectX.Initialize(handle, aWidth, aHeight);
	if (!result)
	{
		std::cout << "Failed to initialize DirectX" << std::endl;
		std::cin.get();
		return false;
	}

	result = myImGuiLayer.Initialize(handle);
	if (!result)
	{
		std::cout << "Failed to initialize ImGui Layer" << std::endl;
		std::cin.get();
		return false;
	}

	return true;
}

void Engine::Update()
{
	if (myWindowContainer.ProcessMessages())
	{
		myDirectX.BeginFrame();
		myImGuiLayer.Begin();
		myImGuiLayer.End();
		myDirectX.EndFrame();
	}
	else
	{
		myIsRunning = false;
	}
}
