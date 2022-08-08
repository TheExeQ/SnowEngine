#pragma once
#include <vector>

namespace Snow 
{
	class ScriptableComponent
	{
	public:
		ScriptableComponent() { sScriptableComponents.emplace_back(this); }
		virtual ~ScriptableComponent() {}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}
		
	private:
		friend class Scene;
		inline static std::vector<ScriptableComponent*> sScriptableComponents;
	};
}
