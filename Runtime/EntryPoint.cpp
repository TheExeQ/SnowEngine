#include <Engine/Engine.h>
#include "GameLayer.h"

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
	Snow::GameLayer game;

	if (engine.Initialize(hInstance, 1280, 720))
	{
		// Initialization
		engine.PushLayer(&game);

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