#pragma once
#include "entt/entt.hpp"
#include "Components.h"
#include "Scene.h"

namespace Snow
{
	class Entity
	{
	public:
		Entity() {};
		Entity(entt::entity aEntityHandle, Scene* aScene);

		inline bool IsValid() 
		{
			if (!myScene) { return false; }
			else
			{
				return myScene->myRegistry.valid(myEntityHandle);
			}
		}

		inline entt::entity GetHandle() 
		{
			return myEntityHandle;
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
		Scene* myScene = NULL;
	};
}