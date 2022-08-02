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
	enum class EngineRunMode
	{
		Editor,
		Game
	};

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

		void SetRunMode(EngineRunMode aMode) { myRunMode = aMode; };
		EngineRunMode GetRunMode() { return myRunMode; };
		bool IsRunning() const { return myIsRunning; };

		static Engine& Get() { return *myInstance; };

		static WindowContainer* GetWindowContainer() { return &Get().myWindowContainer; };
		static Scene* GetActiveScene() { return &Get().myActiveScene; }
		static void SetActiveCamera(Camera* aCamera) { Get().myRenderer.myMainCamera = aCamera; }

	private:
		friend class SceneViewportPanel;
		friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		WindowContainer myWindowContainer;
		Renderer myRenderer;
		LayerStack myLayerStack;
		ImGuiLayer myImGuiLayer;
		Time myTime;

		Scene myActiveScene;

		bool myIsRunning = true;
		EngineRunMode myRunMode = EngineRunMode::Game;

		inline static Engine* myInstance = nullptr;
	};
}
