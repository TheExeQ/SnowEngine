#pragma once
#include <windows.h>
#include "Win32/WindowContainer.h"
#include "Renderer/Renderer.h"
#include "imgui/ImGuiLayer.h"
#include "Core/LayerStack.h"
#include "Core/Time.h"
#include "Scene/Scene.h"

namespace Snow
{
	class Engine
	{
	public:
		bool Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight,
			std::string aWindowTitle = "Snow Engine", const std::string& aWindowClass = "Snow Engine");
		void Update();
		void CleanUp();

		void PushLayer(Layer* aLayer);
		void PushOverlay(Layer* aLayer);

		void EnableVSync() { myRenderer.myVSyncEnabled = true; };
		void DisableVSync() { myRenderer.myVSyncEnabled = false; };

		bool IsRunning() const { return myIsRunning; };

		static Engine& Get() { return *myInstance; };

		static WindowContainer* GetWindowContainer() { return &Get().myWindowContainer; };
		static Scene* GetActiveScene() { return &Get().myActiveScene; }

	private:
		friend class DX11;
		friend class WindowContainer;
		friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		WindowContainer myWindowContainer;
		Renderer myRenderer;
		LayerStack myLayerStack;
		ImGuiLayer myImGuiLayer;
		Time myTime;

		Scene myActiveScene;

		bool myIsRunning = true;

		inline static Engine* myInstance = nullptr;
	};
}
