#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Scene/Components.h"

#include <entt/entt.hpp>
#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>
#include <utility>

namespace Snow
{
	using EntityMap = std::unordered_map<UUID, entt::entity>;
	
	class Renderer;
	class Entity;
	class Camera;

	enum class SceneState
	{
		Edit,
		Play,
		Pause,
		Simulate
	};

	class Scene
	{
	public:
		Scene();
		~Scene();
		
		void CopyTo(Ref<Scene> aScene);

		std::string GetName() const { return mySceneName; };
		void SetName(const std::string& aName) { mySceneName = aName; };

		void LoadScene(const char* aFilePath);
		void ClearScene();
		
		Entity GetEntityFromUUID(UUID aID);
		Entity GetEntityFromName(std::string aName);

		void ParentEntity(Entity aChild, Entity aParent);
		void UnparentEntity(Entity aEntity);

		Entity CreateEntity(const char* aName, Ref<Scene> aScene = nullptr);
		Entity CreateEntityWithID(UUID aID, const char* aName, Ref<Scene> aScene = nullptr);
		void DestroyEntity(Entity aEntity);

		Entity GetPrimaryCameraEntity();

		glm::mat4 GetWorldSpaceTransformMatrix(Entity aEntity);
		glm::mat4 GetLocalSpaceTransformMatrix(Entity aEntity);

		void SetSceneState(SceneState aState) { mySceneState = aState; }
		SceneState GetSceneState() const { return mySceneState; }
		
		void Update();

		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnUpdateRuntime();

	private:
		friend class Entity;
		friend class Camera;
		friend class SceneRenderer;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

		void ConvertToWorldSpace(Entity aEntity);
		void ConvertToLocalSpace(Entity aEntity);

		std::string mySceneName = "Untitled";
		SceneState mySceneState = SceneState::Edit;
		entt::registry myRegistry;
		EntityMap myEnttMap;
	};
}