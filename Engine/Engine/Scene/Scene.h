#pragma once
#include "entt/entt.hpp"
#include "Components.h"
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
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		std::vector<std::pair<const TransformComponent*, const StaticMeshComponent*>> RenderScene(const Camera& aCamera) const; // Return entitys to render

		std::string mySceneName = "Untitled";
		entt::registry myRegistry;
	};
}