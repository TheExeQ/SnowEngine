#pragma once
#include "ScriptableEntity.h"
#include "Engine/Debug/Log.h"

// ADDING A NEW COMPONENT
// ----------------------
// If you add a new type of component, there are several pieces of code that need updating:
// 1) Add new component here (obviously).
// 2) If appropriate, update SceneHierarchy panel to render the new component, and to allow new component to be added (via "Add Component" menu).
// 3) Update SceneSerializer to serialize and deserialize the new component.

namespace Snow
{
	struct TestComponent : public ScriptableEntity
	{
		void OnUpdate() override { LOG_INFO("testUpdate"); }
	};
}