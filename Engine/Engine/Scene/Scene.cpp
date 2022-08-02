#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"

#include "Engine/Renderer/Camera.h"

namespace Snow
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::LoadScene(const char* aFilePath)
	{
		SceneSerializer serializer(this);
		serializer.Deserialize(aFilePath);
	}

	Entity Scene::CreateEntity(const char* aName)
	{
		Entity entity(myRegistry.create(), this);
		entity.AddComponent<TagComponent>()->name = aName;
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

	std::vector<std::pair<const TransformComponent*, const StaticMeshComponent*>> Scene::RenderScene(Camera* aCamera) const
	{
		if (!aCamera)
		{
			myRegistry.each([&](entt::entity entity)
				{
					auto comp = myRegistry.try_get<CameraComponent>(entity);
					if (comp)
					{
						auto position = myRegistry.try_get<TransformComponent>(entity)->position;
						//aCamera = &comp->camera;
						//aCamera->SetPosition(position.x, position.y, position.z);
					}
				});
		}

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
}