#include "Engine.h"
#include "Engine/Debug/Log.h"

namespace Snow
{
	bool Engine::Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight, std::string aWindowTitle, const std::string& aWindowClass)
	{
		Log::Init();

		if (!myInstance)
		{
			myInstance = this;
		}
		else
		{
			CORE_LOG_ERROR("Engine already initialized");
			return false;
		}

		myActiveScene = CreateRef<Scene>();
		myEditorScene = CreateRef<Scene>();

		auto result = myWindowContainer.Initialize(hInstance, aWidth, aHeight, aWindowTitle, aWindowClass);
		if (!result)
		{
			CORE_LOG_ERROR("Failed to initialize Win32");
			return false;
		}

		HWND handle = myWindowContainer.GetWindowHandle();

		result = myRenderer.Initialize(handle);
		if (!result)
		{
			CORE_LOG_ERROR("Failed to initialize Renderer");
			return false;
		}

		myImGuiLayer.OnAttach();
		return true;
	}

	void Engine::Update()
	{
		if (myIsRunning)
		{
			myTime.Update();
			myWindowContainer.ProcessMessages();

			for (auto* layer : myLayerStack)
			{
				layer->OnUpdate();
			}

			myRenderer.BeginFrame();
			if (GetActiveWindow())
			{
				myImGuiLayer.Begin();
#ifndef RETAIL
				for (auto* layer : myLayerStack)
				{
					layer->OnImGuiRender();
				}
#endif // !RETAIL
				myImGuiLayer.End();
			}
			myRenderer.EndFrame();
		}
	}

	void Engine::CleanUp()
	{
		myImGuiLayer.OnDetach();
	}

	void Engine::PushLayer(Layer* aLayer)
	{
		myLayerStack.PushLayer(aLayer);
		aLayer->OnAttach();
	}

	void Engine::PushOverlay(Layer* aLayer)
	{
		myLayerStack.PushOverlay(aLayer);
		aLayer->OnAttach();
	}
}