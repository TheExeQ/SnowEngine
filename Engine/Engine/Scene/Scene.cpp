#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"

#include "Engine/Engine.h"
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
		myRegistry.destroy(aEntity);
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
		if (!aCamera || !aCamera->GetIsPrimary())
		{
			auto cameraEntities = myRegistry.view<CameraComponent>();
			
			for (auto entity : cameraEntities)
			{
				if (myRegistry.get<CameraComponent>(entity).camera.GetIsPrimary())
				{
					Entity primaryCameraEntity(entity, Engine::GetActiveScene());
					auto pos = primaryCameraEntity.GetComponent<TransformComponent>()->position;
					auto& camera = primaryCameraEntity.GetComponent<CameraComponent>()->camera;
					camera.SetPosition(pos);
					Engine::SetActiveCamera(&camera);
				}
			}
		}
		
		std::vector<std::pair<const TransformComponent*, const StaticMeshComponent*>> entitiesToRender;
		auto entitesWithComponents = myRegistry.view<TransformComponent, StaticMeshComponent>();
		
		for (auto entity : entitesWithComponents)
		{
			auto [transform, staticMesh] = myRegistry.get<TransformComponent, StaticMeshComponent>(entity);
			entitiesToRender.push_back(std::pair<const TransformComponent*, const StaticMeshComponent*>(&transform, &staticMesh));
		}
		
		return entitiesToRender;
	}
}