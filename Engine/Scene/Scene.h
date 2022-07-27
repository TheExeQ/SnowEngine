#pragma once
#include "entt/entt.hpp"
#include "Components.h"
#include <vector>
#include <utility>

class Renderer;
class Entity;
class Camera;

class Scene
{
public:
	Scene();
	~Scene();

	Entity CreateEntity();
	void DestroyEntity(Entity aEntity);

	void OnRuntimeStart();
	void OnRuntimeStop();

	void OnSimulationStart();
	void OnSimulationStop();

	void OnUpdateRuntime();
	void OnUpdateSimulation();
	void OnUpdateEditor();

private:
	friend class Renderer;
	friend class Entity;

	std::vector<std::pair<const TransformComponent*, const StaticMeshComponent*>> RenderScene(const Camera& aCamera) const; // Return entitys to render

	entt::registry myRegistry;
};
