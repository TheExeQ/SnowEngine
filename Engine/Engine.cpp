#include "Engine.h"
#include <iostream>

namespace Snow
{
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

		myImGuiLayer.OnAttach();

		return true;
	}

	void Engine::Update()
	{
		if (myIsRunning)
		{
			myTime.Update();
			myWindowContainer.ProcessMessages();
			if (!myIsRunning) { myImGuiLayer.myLayerActive = false; };

			for (auto* layer : myLayerStack)
			{
				layer->OnUpdate();
			}

			myRenderer.BeginFrame();
			if (myImGuiLayer.IsLayerActive())
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