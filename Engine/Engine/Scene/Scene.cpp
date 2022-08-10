#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"
#include "Engine/Math/Math.h"

#include "Engine/Engine.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Scene/ScriptableEntity.h"

#include <entt/entt.hpp>

namespace Snow
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	template<typename T>
	static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto components = srcRegistry.view<T>();
		for (auto srcEntity : components)
		{
			entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).uuid);

			auto& srcComponent = srcRegistry.get<T>(srcEntity);
			auto& destComponent = dstRegistry.emplace_or_replace<T>(destEntity, srcComponent);
		}
	}

	void Scene::CopyTo(Ref<Scene> aScene)
	{
		aScene->ClearScene();
		
		auto view = myRegistry.view<IDComponent>();
		for (entt::entity entity : view)
		{
			auto name = myRegistry.get<TagComponent>(entity).name;
			auto id = myRegistry.get<IDComponent>(entity).uuid;
			aScene->CreateEntityWithID(id, name.c_str(), aScene);
		}

		CopyComponent<NativeScriptComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<TagComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<IDComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<RelationshipComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<TransformComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<CameraComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<StaticMeshComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
		CopyComponent<SkeletalMeshComponent>(aScene->myRegistry, myRegistry, aScene->myEnttMap);
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
		if (auto it = myEnttMap.find(aID); it != myEnttMap.end())
		{
			return Entity(it->second, this);
		}
		return Entity();
	}

	Snow::Entity Scene::GetEntityFromName(std::string aName)
	{
		auto view = myRegistry.view<TagComponent>();
		for (entt::entity entity : view)
		{
			if (view.get<TagComponent>(entity).name == aName)
			{
				return Entity(entity, this);
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
		Entity entity(myRegistry.create(), (aScene) ? aScene.get() : Engine::GetActiveScene().get());
		entity.AddComponent<TagComponent>()->name = aName;
		auto uuid = entity.AddComponent<IDComponent>()->uuid;
		entity.AddComponent<RelationshipComponent>();
		entity.AddComponent<TransformComponent>();
		myEnttMap[uuid] = (entt::entity)entity;
		return entity;
	}

	Snow::Entity Scene::CreateEntityWithID(UUID aID, const char* aName, Ref<Scene> aScene /*= nullptr*/)
	{
		Entity entity(myRegistry.create(), (aScene) ? aScene.get() : Engine::GetActiveScene().get());
		entity.AddComponent<TagComponent>()->name = aName;
		entity.AddComponent<IDComponent>()->uuid = aID;
		entity.AddComponent<RelationshipComponent>();
		entity.AddComponent<TransformComponent>();
		myEnttMap[aID] = (entt::entity)entity;
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

	Snow::Entity Scene::GetPrimaryCameraEntity()
	{
		auto cameraEntities = myRegistry.view<CameraComponent>();
		for (auto entity : cameraEntities)
		{
			if (myRegistry.get<CameraComponent>(entity).camera.GetIsPrimary())
			{
				return Entity(entity, this);
			}
		}

		return Entity(this);
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

		if (aEntity.HasParent())
		{
			Entity parent(aEntity.ParentUUID());
			if (parent.IsValid())
			{
				transform = GetWorldSpaceTransformMatrix(parent);
			}
		}

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

	void Scene::Update()
	{
		if (mySceneState == SceneState::Edit || mySceneState == SceneState::Pause) { return; }

		OnUpdateRuntime();
	}

	void Scene::OnRuntimeStart()
	{
		// Init scripts
		myRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance && nsc.scriptID >= 0)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->myEntity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
			});
	}

	void Scene::OnRuntimeStop()
	{
		// Destroy scripts
		myRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (nsc.Instance)
				{
					nsc.Instance->OnDestroy();
				}
			});
	}

	void Scene::OnUpdateRuntime()
	{
		// Update scripts
		myRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (nsc.Instance)
				{
					nsc.Instance->OnUpdate();
				}
			});
	}
}