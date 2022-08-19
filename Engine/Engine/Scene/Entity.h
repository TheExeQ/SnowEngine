#pragma once
#include "Engine/Core/Base.h"
#include "Components.h"
#include "Scene.h"

#include <entt/entt.hpp>

namespace Snow
{
	class Entity
	{
	public:
		Entity();
		Entity(UUID aID);
		Entity(UUID aID, Scene* aScene);
		Entity(entt::entity aEntityHandle);
		Entity(entt::entity aEntityHandle, Scene* aScene);
		Entity(Scene* aScene);
		Entity(const Entity& aEntity);

		UUID GetUUID();
		UUID ParentUUID();
		bool HasParent();
		std::vector<UUID> ChildrenUUIDs();
		glm::mat4 GetTransform();
		glm::mat4 GetWorldTransform();
		bool IsValid();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>()) { return GetComponent<T>(); }
			return myScene->myRegistry.emplace<T>(myEntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return myScene->myRegistry.get<T>(myEntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!HasComponent<T>()) { return; }
			myScene->myRegistry.remove<T>(myEntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return (myScene->myRegistry.valid(myEntityHandle)) ? myScene->myRegistry.all_of<T>(myEntityHandle) : false;
		}

		operator entt::entity() const { return myEntityHandle; }
		operator uint32_t() const { return (uint32_t)myEntityHandle; }

		bool operator==(const Entity& other) const
		{
			return myEntityHandle == other.myEntityHandle && myScene == other.myScene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity myEntityHandle = entt::null;
		Scene* myScene = nullptr;
	};
}