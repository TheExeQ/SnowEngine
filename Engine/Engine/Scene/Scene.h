#pragma once
#include "entt/entt.hpp"
#include "Components.h"

#include <glm/glm.hpp>
#include <vector>
#include <utility>

namespace Snow
{
	class Renderer;
	class Entity;
	class Camera;

	class Scene
	{
	public:
		Scene();
		~Scene();

		std::string GetName() const { return mySceneName; };
		void SetName(const std::string& aName) { mySceneName = aName; };

		void LoadScene(const char* aFilePath);
		
		void ParentEntity(Entity aChild, Entity aParent);
		void UnparentEntity(Entity aEntity);

		Entity CreateEntity(const char* aName);
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
		friend class Entity;
		friend class Camera;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		void ConvertToWorldSpace(Entity aEntity);
		void ConvertToLocalSpace(Entity aEntity);
		glm::mat4 GetWorldSpaceTransformMatrix(Entity aEntity);

		std::vector<Entity> RenderScene(Ref<Camera> aCamera); // Return entitys to render

		std::string mySceneName = "Untitled";
		entt::registry myRegistry;
	};
}