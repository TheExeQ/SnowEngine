#include <Engine.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	Engine engine;

	if (engine.Initialize(hInstance, 1280, 720))
	{
		while (engine.IsRunning())
		{
			engine.Update();
		}
		engine.CleanUp();
	}
	return 0;
}