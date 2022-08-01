#include <Engine/Engine.h>
#include <cstdio>
#include <iostream>

#include "EditorLayer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
	
	Snow::Engine engine;
	engine.SetRunMode(Snow::EngineRunMode::Editor);
	Snow::EditorLayer editor;
	
	if (engine.Initialize(hInstance, 1280, 720))
	{
		// Initialization
		engine.PushLayer(&editor);

		// Update Loop
		while (engine.IsRunning())
		{
			engine.Update();
		}

		// CleanUp
		engine.CleanUp();
	}
	return 0;
}