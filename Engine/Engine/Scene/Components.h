#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Skeleton.h"
#include "Engine/Renderer/Material.h"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

// ADDING A NEW COMPONENT
// ----------------------
// If you add a new type of component, there are several pieces of code that need updating:
// 1) Add new component here (obviously).
// 2) If appropriate, update SceneHierarchy panel to render the new component, and to allow new component to be added (via "Add Component" menu).
// 3) Update SceneSerializer to serialize and deserialize the new component.
// 4) Update CopyTo in Scene to make sure component won't be edited during play/simulations.

namespace Snow
{
	// Forward declaration
	class ScriptableEntity;
	
	struct NativeScriptComponent
	{
		int scriptID = -1;
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}

		template<typename T>
		T* CastTo()
		{
			return static_cast<T*>(Instance);
		}
	};

	struct TagComponent
	{
		std::string name;
	};

	struct IDComponent
	{
		UUID uuid = UUID();
	};

	struct RelationshipComponent
	{
		UUID Parent = 0;
		std::vector<UUID> Children;
	};

	struct TransformComponent
	{
		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec3 rotation = { 0.f, 0.f, 0.f };
		glm::vec3 scale = { 1.f, 1.f, 1.f };

		glm::mat4 GetTransform() const
		{
			glm::mat4 positionMatrix = glm::translate(glm::mat4(1.f), position);
			glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotation));
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

			return positionMatrix * rotationMatrix * scaleMatrix;
		}
		
		glm::vec3 Right() const
		{
			return glm::normalize(-GetTransform()[0]);
		}

		glm::vec3 Up() const
		{
			return glm::normalize(-GetTransform()[1]);
		}

		glm::vec3 Forward() const
		{
			return glm::normalize(-GetTransform()[2]);
		}
	};

	struct CameraComponent
	{
		Camera camera;
	};

	struct StaticMeshComponent
	{
		Model model;
		Material material;
	};

	struct SkeletalMeshComponent
	{
		Model animatedModel;
		Material material;
		Skeleton skeleton;
	};
}