#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"
#include "Engine/Math/Math.h"

#include "Engine/Engine.h"
#include "Engine/Renderer/Camera.h"

#include <entt/entt.hpp>

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

	void Scene::ParentEntity(Entity aChild, Entity aParent)
	{
		UnparentEntity(aChild);
		
		const auto& childRelationShip = aChild.GetComponent<RelationshipComponent>();
		const auto& parentRelationShip = aParent.GetComponent<RelationshipComponent>();
		
		childRelationShip->Parent = aParent;
		parentRelationShip->Children.push_back(aChild);

		ConvertToLocalSpace(aChild);
	}

	void Scene::UnparentEntity(Entity aEntity)
	{
		const auto& entityRelationShip = aEntity.GetComponent<RelationshipComponent>();
		if (aEntity.HasParent())
		{
			auto& parentChildren = myRegistry.get<RelationshipComponent>(aEntity.Parent()).Children;
			parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), (entt::entity)aEntity), parentChildren.end());
		}

		ConvertToWorldSpace(aEntity);

		entityRelationShip->Parent = entt::null;
	}

	Entity Scene::CreateEntity(const char* aName)
	{
		Entity entity(myRegistry.create(), this);
		entity.AddComponent<TagComponent>()->name = aName;
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<RelationshipComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity aEntity)
	{
		UnparentEntity(aEntity);
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

	void Scene::ConvertToWorldSpace(Entity aEntity)
	{
		Entity parent(aEntity.Parent(), this);

		if (!parent.IsValid())
			return;

		glm::mat4 transform = GetWorldSpaceTransformMatrix(aEntity);
		const auto& entityTransformComp = aEntity.GetComponent<TransformComponent>();
		Math::DecomposeTransform(transform, entityTransformComp->position, entityTransformComp->rotation, entityTransformComp->scale);
	}

	void Scene::ConvertToLocalSpace(Entity aEntity)
	{
		Entity parent(aEntity.Parent(), this);

		if (!parent.IsValid())
			return;

		glm::mat4 transform = aEntity.Transform();
		glm::mat4 parentTransform = GetWorldSpaceTransformMatrix(parent);
		
		glm::mat4 localTransform = glm::inverse(parentTransform) * transform;

		const auto& entityTransformComp = aEntity.GetComponent<TransformComponent>();
		Math::DecomposeTransform(localTransform, entityTransformComp->position, entityTransformComp->rotation, entityTransformComp->scale);
	}

	glm::mat4 Scene::GetWorldSpaceTransformMatrix(Entity aEntity)
	{
		glm::mat4 transform(1.0f);

		Entity parent(aEntity.Parent(), this);
		if (parent.IsValid())
			transform = GetWorldSpaceTransformMatrix(parent);

		return transform * aEntity.Transform();
	}

	std::vector<Entity> Scene::RenderScene(Camera* aCamera)
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
		
		std::vector<Entity> entitiesToRender;
		auto entitesWithComponents = myRegistry.view<TransformComponent, RelationshipComponent, StaticMeshComponent>();
		
		for (auto entity : entitesWithComponents)
		{
			entitiesToRender.push_back(Entity(entity, this));
		}
		
		return entitiesToRender;
	}
}