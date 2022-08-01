#pragma once
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Editor/SceneViewportPanel.h"

#include "Engine/Scene/SceneSerializer.h"

#include <Engine/Core/Layer.h>
#include <Engine/Scene/Entity.h>
#include <string>

namespace Snow
{
	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnImGuiRender() override;

	private:
		void RenderDockspace();

		std::string myLastLoadedScene = "";

		// Panels
		SceneHierarchyPanel mySceneHierarchyPanel;
		SceneViewportPanel mySceneViewportPanel;
	};
}
