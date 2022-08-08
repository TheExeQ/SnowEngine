#pragma once
#include "Entity.h"

namespace Snow 
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return *myEntity.GetComponent<T>();
		}
		
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}
		
	private:
		friend class Scene;
		Entity myEntity;
	};
}
