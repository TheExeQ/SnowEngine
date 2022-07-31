#pragma once
#include "Panels/SceneHierarchyPanel.h"
#include "Editor/SceneViewportPanel.h"

#include "Scene/SceneSerializer.h"

#include <Core/Layer.h>
#include <Scene/Entity.h>

namespace Snow
{
	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnImGuiRender() override;
		virtual void OnUpdate() override;

	private:
		Entity myCube;
		Entity myChest;
		Entity myPyramid;

		// Panels
		SceneHierarchyPanel mySceneHierarchyPanel;
		SceneViewportPanel mySceneViewportPanel;
	};
}
