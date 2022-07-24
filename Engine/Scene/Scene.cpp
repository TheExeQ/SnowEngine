#include "Scene.h"
#include "Entity.h"
#include "Components.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

Entity Scene::CreateEntity()
{
	Entity entity(myRegistry.create(), this);
	entity.AddComponent<TransformComponent>();
	return entity;
}

void Scene::DestroyEntity(Entity aEntity)
{
	
}

void Scene::OnRuntimeStart()
{

}

void Scene::OnRuntimeStop()
{

}

void Scene::OnSimulationStart()
{

}

void Scene::OnSimulationStop()
{

}

void Scene::OnUpdateRuntime()
{

}

void Scene::OnUpdateSimulation()
{

}

void Scene::OnUpdateEditor()
{

}

void Scene::RenderScene() const
{
	
}
