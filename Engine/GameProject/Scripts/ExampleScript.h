#pragma once
#include "Engine/Core/Fundamentals.h"
#include "Engine/Scene/ScriptableEntity.h"

// ADDING NEW SCRIPT
// ----------------------
// 1. Copy this file and rename class to the name of your script.
// 2. Add your script to the Scripts.h file.
// 3. Goto SceneHierarchyPanel.cpp and add your script to the items list inside DrawComponent<NativeScriptComponent>().
// 4. Also inside SceneHierarchyPanel.cpp add a case to the switch statement in BindNativeScript() and change the bind to your script.
// IMPORTANT: the case number must match the index of your script in the items list from step 3.

// ADDITIONAL INFORMATION
// ----------------------
// It is not a requirement to write all the code in the header, it is completly fine to split it up into a cpp and that is also recommended since it will avoid potential problems.
// Inside ScriptableEntity class you can find all virtual functions that can be overwritten in your script.
// To access other components on your entity call the GetComponent<ComponentType>() function derived from ScriptableEntity.
// Currently you can not add script variables to the editor properties panel nor will values be saved to the scene file so make sure to set the values of your variables.

using namespace Snow;

namespace Game
{
	class ExampleScript : public ScriptableEntity
	{
	public:
		void OnCreate() { LOG_INFO("ExampleScript::OnCreate"); };
		void OnUpdate() { LOG_INFO("ExampleScript::OnUpdate"); };
		void OnDestroy() { LOG_INFO("ExampleScript::OnDestroy"); };
	};
}
