#pragma once

class Renderer;
class Entity;

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

	void RenderScene() const; // Return entitys to render
};
