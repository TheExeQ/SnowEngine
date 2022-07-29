#pragma once
#include <Scene/Entity.h>

class SceneHierarchyPanel
{
public:
	void Init();
	void OnImGuiRender();

private:
	void DrawEntityNode(Entity entity);

	Scene* myContext = nullptr;
	Entity mySelectionContext;
};