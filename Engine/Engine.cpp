#include "Engine.h"
#include <iostream>

bool Engine::Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight, std::string aWindowTitle, const std::string& aWindowClass)
{
	if (!myInstance)
	{
		myInstance = this;
	}
	else
	{
		std::cout << "Engine already initialized" << std::endl;
		std::cin.get();
		return false;
	}

	auto result = myWindowContainer.Initialize(hInstance, aWidth, aHeight, aWindowTitle, aWindowClass);
	if (!result)
	{
		std::cout << "Failed to initialize Win32" << std::endl;
		std::cin.get();
		return false;
	}

	HWND handle = myWindowContainer.GetWindowHandle();

	result = myRenderer.Initialize(handle, aWidth, aHeight);
	if (!result)
	{
		std::cout << "Failed to initialize Renderer" << std::endl;
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

void Engine::Begin()
{
	if (myIsRunning)
	{
		myWindowContainer.ProcessMessages();
		myRenderer.BeginFrame();
		myImGuiLayer.Begin();
	}
}

void Engine::End()
{
	if (myIsRunning)
	{
		myImGuiLayer.End();
		myRenderer.EndFrame();
	}
}

void Engine::CleanUp()
{
	myImGuiLayer.CleanUp();
}