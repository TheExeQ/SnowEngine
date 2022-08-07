#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/DX11.h"
#include "Engine/Scene/Entity.h"
#include "SceneRenderer.h"

namespace Snow
{
	class Renderer
	{
	private:
		friend class Engine;
		friend class SceneViewportPanel;

		bool Initialize(HWND hwnd);
		void BeginFrame();
		void EndFrame();

		Ref<SceneRenderer> mySceneRenderer = CreateRef<SceneRenderer>();

		DX11 myGraphicsAPI;
		bool myVSyncEnabled = true;
	};
}