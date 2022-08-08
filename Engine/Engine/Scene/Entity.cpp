#include "Entity.h"
#include <Engine/Engine.h>

namespace Snow
{
	Entity::Entity()
	{
		myScene = Engine::GetActiveScene().get();
	}
	
	Entity::Entity(entt::entity aEntityHandle, Scene* aScene)
		: myEntityHandle(aEntityHandle), myScene(aScene)
	{

	}

	Entity::Entity(entt::entity aEntityHandle)
		: myEntityHandle(aEntityHandle)
	{
		myScene = Engine::GetActiveScene().get();
	}

	Entity::Entity(const Entity& aEntity)
	{
		myEntityHandle = aEntity.myEntityHandle;
		myScene = aEntity.myScene;
	}

	Entity::Entity(UUID aID)
	{
		myScene = Engine::GetActiveScene().get();

		myEntityHandle = (entt::entity)myScene->GetEntityFromUUID(aID);
	}

	Entity::Entity(UUID aID, Scene* aScene)
		: myScene(aScene)
	{
		myEntityHandle = (entt::entity)myScene->GetEntityFromUUID(aID);
	}

	Entity::Entity(Scene* aScene)
		: myScene(aScene)
	{

	}

	UUID Entity::GetUUID()
	{
		return GetComponent<IDComponent>()->uuid;
	}

	std::vector<UUID> Entity::ChildrenUUIDs()
	{
		return GetComponent<RelationshipComponent>()->Children;
	}

	UUID Entity::ParentUUID()
	{
		return GetComponent<RelationshipComponent>()->Parent;
	}

	bool Entity::HasParent()
	{
		return ParentUUID();
	}

	glm::mat4 Entity::GetTransform()
	{
		return GetComponent<TransformComponent>()->GetTransform();
	}

	glm::mat4 Entity::GetWorldTransform()
	{
		return myScene->GetWorldSpaceTransformMatrix(*this);
	}

	bool Entity::IsValid()
	{
		if (!myScene) { return false; }
		else
		{
			return myScene->myRegistry.valid(myEntityHandle);
		}
	}
}