#pragma once
#include "entt/entt.hpp"

class Scene;

class Entity
{
public:
	Entity(entt::entity aEntityHandle, Scene* aScene);

	inline entt::entity get_handle() { return myEntityHandle;
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return myScene->registry.emplace<T>(myEntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		return myScene->registry.get<T>(myEntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		return myScene->registry.remove<T>(myEntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return myScene->registry.all_of<T>(myEntityHandle);
	}

private:
	entt::entity myEntityHandle = entt::null;
	Scene* myScene = NULL;
};

