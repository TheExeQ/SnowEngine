#pragma once
#include <windows.h>
#include "Win32/WindowContainer.h"
#include "Graphics/DX11.h"

class Engine
{
public:
	bool Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight, 
		std::string aWindowTitle = "Snow Engine", const std::string& aWindowClass = "Snow Engine");
	void Update();
	
	bool IsRunning() const { return myIsRunning; };

private:
	bool myIsRunning = true;
	
	WindowContainer myWindowContainer;
	DX11 myDirectX;
};

