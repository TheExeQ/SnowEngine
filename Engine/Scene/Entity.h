#pragma once
#include "entt/entt.hpp"
#include "Components.h"
#include "Scene/Scene.h"

class Entity
{
public:
	Entity(entt::entity aEntityHandle, Scene* aScene);

	inline entt::entity GetHandle() { return myEntityHandle;
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
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
		return myScene->myRegistry.remove<T>(myEntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return myScene->myRegistry.all_of<T>(myEntityHandle);
	}

private:
	entt::entity myEntityHandle = entt::null;
	Scene* myScene = NULL;
};
