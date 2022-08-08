#pragma once
#include "Engine/Core/Base.h"
#include "Components.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <utility>

namespace Snow
{
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

		std::string GetName() const { return mySceneName; };
		void SetName(const std::string& aName) { mySceneName = aName; };

		void LoadScene(const char* aFilePath);
		void ClearScene();
		
		Entity GetEntityFromUUID(UUID aID);

		void ParentEntity(Entity aChild, Entity aParent);
		void UnparentEntity(Entity aEntity);

		Entity CreateEntity(const char* aName, Ref<Scene> aScene = nullptr);
		void DestroyEntity(Entity aEntity);

		Entity GetPrimaryCameraEntity();

		glm::mat4 GetWorldSpaceTransformMatrix(Entity aEntity);
		glm::mat4 GetLocalSpaceTransformMatrix(Entity aEntity);

		SceneState GetSceneState() const { return mySceneState; }
		
		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateRuntime();
		void OnUpdateSimulation();
		void OnUpdateEditor();

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
	};
}