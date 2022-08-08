#pragma once
#include <windows.h>
#include "Core/Base.h"
#include "Core/LayerStack.h"
#include "Win32/WindowContainer.h"
#include "Renderer/Renderer.h"
#include "imgui/ImGuiLayer.h"
#include "Scene/Scene.h"

namespace Snow
{
	enum class EngineRunMode
	{
		Editor,
		Game
	};

	class Engine // #TODO: make relative path variable so you can rename bin folder and putting assets inside bin folder without game crashing.
	{
	public:
		bool Initialize(HINSTANCE hInstance, const int& aWidth, const int& aHeight,
			std::string aWindowTitle = "Snow Engine", const std::string& aWindowClass = "Snow Engine");
		void Update();
		void CleanUp();

		void PushLayer(Layer* aLayer);
		void PushOverlay(Layer* aLayer);

		static void EnableVSync() { Get().myRenderer.myVSyncEnabled = true; };
		static void DisableVSync() { Get().myRenderer.myVSyncEnabled = false; };

		void SetRunMode(EngineRunMode aMode) { myRunMode = aMode; };
		EngineRunMode GetRunMode() { return myRunMode; };
		bool IsRunning() { return myIsRunning; };

		static Engine& Get() { return *myInstance; };

		static WindowContainer* GetWindowContainer() { return &Get().myWindowContainer; };
		static Ref<Scene> GetActiveScene() { return Get().myActiveScene; }
		static Ref<Scene> GetEditorScene() { return Get().myEditorScene; }
		
		static void SetEditorCamera(Entity aCameraEntity) { Get().myRenderer.mySceneRenderer->UpdateEditorCamera(aCameraEntity); };
		static void SetRuntimeCamera(Entity aCameraEntity) { Get().myRenderer.mySceneRenderer->UpdateRuntimeCamera(aCameraEntity); };

	private:
		friend class SceneViewportPanel;
		friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		WindowContainer myWindowContainer;
		Renderer myRenderer;
		LayerStack myLayerStack;
		ImGuiLayer myImGuiLayer;
		Time myTime;

		Ref<Scene> myActiveScene;
		Ref<Scene> myEditorScene;

		bool myIsRunning = true;
		EngineRunMode myRunMode = EngineRunMode::Game;

		inline static Engine* myInstance = nullptr;
	};
}
