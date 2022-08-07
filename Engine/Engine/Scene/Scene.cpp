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
		SceneSerializer serializer(Engine::GetActiveScene());
		serializer.Deserialize(aFilePath);
	}

	void Scene::ClearScene()
	{
		myRegistry.clear();
	}

	Snow::Entity Scene::GetEntityFromUUID(UUID aID)
	{
		auto view = myRegistry.view<IDComponent>();
		for (entt::entity entity : view)
		{
			if (view.get<IDComponent>(entity).uuid == aID)
			{
				return Entity(entity);
			}
		}
		return Entity();
	}

	void Scene::ParentEntity(Entity aChild, Entity aParent)
	{
		if (!aChild.IsValid() || !aParent.IsValid()) { return; }
		
		UnparentEntity(aChild);
		
		const auto& childRelationShip = aChild.GetComponent<RelationshipComponent>();
		const auto& parentRelationShip = aParent.GetComponent<RelationshipComponent>();
		
		childRelationShip->Parent = aParent.GetUUID();
		parentRelationShip->Children.push_back(aChild.GetUUID());

		ConvertToLocalSpace(aChild);
	}

	void Scene::UnparentEntity(Entity aEntity)
	{
		if (!aEntity.IsValid()) { return; }
		
		const auto& entityRelationShip = aEntity.GetComponent<RelationshipComponent>();
		if (aEntity.HasParent())
		{
			auto& parentChildren = myRegistry.get<RelationshipComponent>((entt::entity)Entity(aEntity.ParentUUID())).Children;
			parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), aEntity.GetUUID()), parentChildren.end());
		}

		ConvertToWorldSpace(aEntity);

		entityRelationShip->Parent = 0;
	}

	Entity Scene::CreateEntity(const char* aName, Ref<Scene> aScene)
	{
		Entity entity(myRegistry.create(), (aScene) ? aScene : Engine::GetActiveScene());
		entity.AddComponent<TagComponent>()->name = aName;
		entity.AddComponent<IDComponent>();
		entity.AddComponent<RelationshipComponent>();
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity aEntity)
	{
		UnparentEntity(aEntity);
		for (auto child : aEntity.ChildrenUUIDs())
		{
			UnparentEntity(Entity(child));
		}
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
		Entity parent(aEntity.ParentUUID());

		if (!parent.IsValid())
			return;

		glm::mat4 transform = GetWorldSpaceTransformMatrix(aEntity);
		const auto& entityTransformComp = aEntity.GetComponent<TransformComponent>();
		Math::DecomposeTransform(transform, entityTransformComp->position, entityTransformComp->rotation, entityTransformComp->scale);
	}

	void Scene::ConvertToLocalSpace(Entity aEntity)
	{
		Entity parent(aEntity.ParentUUID());

		if (!parent.IsValid())
			return;

		glm::mat4 transform = aEntity.GetTransform();
		glm::mat4 parentTransform = GetWorldSpaceTransformMatrix(parent);
		
		glm::mat4 localTransform = glm::inverse(parentTransform) * transform;

		const auto& entityTransformComp = aEntity.GetComponent<TransformComponent>();
		Math::DecomposeTransform(localTransform, entityTransformComp->position, entityTransformComp->rotation, entityTransformComp->scale);
	}

	glm::mat4 Scene::GetWorldSpaceTransformMatrix(Entity aEntity)
	{
		glm::mat4 transform(1.0f);

		Entity parent(aEntity.ParentUUID());
		if (parent.IsValid())
			transform = GetWorldSpaceTransformMatrix(parent);

		return transform * aEntity.GetTransform();
	}

	glm::mat4 Scene::GetLocalSpaceTransformMatrix(Entity aEntity)
	{
		Entity parent(aEntity.ParentUUID());

		glm::mat4 transform = aEntity.GetTransform();
		
		if (!parent.IsValid())
			return transform;

		glm::mat4 parentTransform = GetWorldSpaceTransformMatrix(parent);

		return glm::inverse(parentTransform) * transform;
	}
}