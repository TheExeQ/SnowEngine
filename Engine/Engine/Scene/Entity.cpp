#include "Entity.h"
#include <Engine/Engine.h>

namespace Snow
{
	Entity::Entity(entt::entity aEntityHandle, Ref<Scene> aScene)
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

	Entity::Entity(UUID aID)
	{
		myScene = Engine::GetActiveScene();

		myEntityHandle = (entt::entity)myScene->GetEntityFromUUID(aID);
	}

	Entity::Entity(UUID aID, Ref<Scene> aScene)
		: myScene(aScene)
	{
		myEntityHandle = (entt::entity)myScene->GetEntityFromUUID(aID);
	}

	Entity::Entity(Ref<Scene> aScene)
		: myScene(aScene)
	{

	}
}