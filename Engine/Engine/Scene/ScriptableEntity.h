#pragma once
#include "Entity.h"

namespace Snow 
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity() { sScriptableEntities.emplace_back(this); }
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return myEntity.GetComponent<T>();
		}
		
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}
		
	private:
		friend class Scene;
		Entity myEntity;
		inline static std::vector<ScriptableEntity*> sScriptableEntities;
	};
}
