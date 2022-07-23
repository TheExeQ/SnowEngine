#include <Engine.h>
#include <cstdio>
#include <iostream>

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

	Engine engine;
	
	if (engine.Initialize(hInstance, 1280, 720))
	{
		while (engine.IsRunning())
		{
			engine.Begin();

			// Remove later only for debug now
			bool show = true;
			ImGui::ShowDemoWindow(&show);

			engine.End();
		}
		engine.CleanUp();
	}
	return 0;
}