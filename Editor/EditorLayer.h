#pragma once
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Editor/SceneViewportPanel.h"

#include "Engine/Scene/SceneSerializer.h"

#include <Engine/Core/Layer.h>
#include <Engine/Scene/Entity.h>

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
