#pragma once
#include <Scene/Entity.h>

namespace Snow
{
	class SceneHierarchyPanel
	{
	public:
		void Init();
		void OnImGuiRender();

	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Scene* myContext = nullptr;
		Entity mySelectionContext;
	};
}
