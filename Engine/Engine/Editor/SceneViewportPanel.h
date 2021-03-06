#pragma once
#include <glm/glm.hpp>
#include "Engine/Scene/Entity.h"

namespace Snow
{
	class SceneViewportPanel
	{
	private:
		friend class EditorLayer;
		friend class WindowContainer;

		void Init();
		void OnImGuiRender(Entity aSelectedEntity);
		void OnResize();

		static SceneViewportPanel& Get() { return *myInstace; }
		
		bool myOpened = true;
		glm::vec2 myViewportSize = {0.f, 0.f};

		inline static SceneViewportPanel* myInstace = nullptr;
	};
}