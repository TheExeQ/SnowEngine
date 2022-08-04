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
		Entity() {};
		Entity(entt::entity aEntityHandle);
		Entity(entt::entity aEntityHandle, Ref<Scene> aScene);
		Entity(const Entity& aEntity);

		inline std::vector<entt::entity> Children()
		{
			return GetComponent<RelationshipComponent>()->Children;
		}

		inline entt::entity Parent()
		{
			return GetComponent<RelationshipComponent>()->Parent;
		}

		inline bool HasParent()
		{
			return myScene->myRegistry.valid(GetComponent<RelationshipComponent>()->Parent);
		}

		inline glm::mat4 Transform()
		{
			return GetComponent<TransformComponent>()->GetTransform();
		}

		inline glm::mat4 GetWorldTransform()
		{
			return myScene->GetWorldSpaceTransformMatrix(*this);
		}

		inline bool IsValid() 
		{
			if (!myScene) { return false; }
			else
			{
				return myScene->myRegistry.valid(myEntityHandle);
			}
		}

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			if (auto comp = GetComponent<T>()) { return comp; }
			return &myScene->myRegistry.emplace<T>(myEntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T* GetComponent()
		{
			if (!HasComponent<T>()) { return nullptr; }
			return &myScene->myRegistry.get<T>(myEntityHandle);
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
		Ref<Scene> myScene = nullptr;
	};
}