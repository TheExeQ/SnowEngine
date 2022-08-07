#pragma once
#include "Engine/Editor/EditorCamera.h"
#include "Engine/Scene/SceneSerializer.h"

// Panels
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Editor/SceneViewportPanel.h"

#include <Engine/Core/Base.h>
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
		virtual void OnUpdate() override;

	private:
		void RenderDockspace();

		Entity myEditorCameraEntity;
		Ref<EditorCamera> myEditorCamera = CreateRef<EditorCamera>();
		std::string myLastLoadedScene = "";

		// Panels
		SceneHierarchyPanel mySceneHierarchyPanel;
		SceneViewportPanel mySceneViewportPanel;
	};
}
