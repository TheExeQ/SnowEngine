#include "Scene.h"
#include "Entity.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

Entity Scene::CreateEntity(const char* aName)
{
	Entity entity(myRegistry.create(), this);
	entity.AddComponent<TagComponent>().name = aName;
	entity.AddComponent<TransformComponent>();
	return entity;
}

void Scene::DestroyEntity(Entity aEntity)
{
	myRegistry.destroy(aEntity.GetHandle());
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

std::vector<std::pair<const TransformComponent*, const StaticMeshComponent*>> Scene::RenderScene(const Camera& aCamera) const
{
	std::vector<std::pair<const TransformComponent*, const StaticMeshComponent*>> entitiesToRender;
	myRegistry.each([&](entt::entity entity)
		{
			auto [transform, staticMesh] = myRegistry.try_get<TransformComponent, StaticMeshComponent>(entity);
			if (transform && staticMesh)
			{
				entitiesToRender.push_back(std::pair<const TransformComponent*, const StaticMeshComponent*>(transform, staticMesh));
			}
		});
	return entitiesToRender;
}
