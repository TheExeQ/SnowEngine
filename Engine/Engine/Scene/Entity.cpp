#include "Entity.h"
#include <Engine/Engine.h>

namespace Snow
{
	Entity::Entity(entt::entity aEntityHandle, Scene* aScene)
		: myEntityHandle(aEntityHandle), myScene(aScene)
	{

	}

	Entity::Entity(entt::entity aEntityHandle)
		: myEntityHandle(aEntityHandle)
	{
		myScene = Engine::GetActiveScene();
	}

	Entity::Entity(const Entity& aEntity)
	{
		myEntityHandle = aEntity.myEntityHandle;
		myScene = aEntity.myScene;
	}
}