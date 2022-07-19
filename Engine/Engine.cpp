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

	result = myDirectX.Initialize(myWindowContainer.GetWindowHandle());
	if (!result)
	{
		std::cout << "Failed to initialize DirectX" << std::endl;
		std::cin.get();
		return false;
	}

	return true;
}

void Engine::Update()
{
	if (myWindowContainer.ProcessMessages())
	{
		myDirectX.RenderFrame();
	}
	else
	{
		myIsRunning = false;
	}
}
