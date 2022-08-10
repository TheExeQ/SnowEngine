#pragma once
#include <Engine/Core/Base.h>
#include <Engine/Scene/Entity.h>

namespace Snow
{
	class SceneHierarchyPanel
	{
	public:
		void OnImGuiRender();
		Entity GetSelectedEntity() const { return mySelectionContext; };
		static void BindNativeScript(Entity aEntity, int aTypeIndex);

	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity aEntity);
		void DrawComponents(Entity aEntity);

		Ref<Scene> myContext = nullptr;
		Entity mySelectionContext;
	};
}
