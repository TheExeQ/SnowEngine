#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Animation.h"
#include "Engine/Renderer/Material.h"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Snow
{
	struct BaseComponent
	{
		BaseComponent() 
		{
			sBaseComponents.emplace_back(this);
		};
		virtual void Start() {};
		virtual void Update() {};

	private:
		friend class Engine;
		inline static std::vector<BaseComponent*> sBaseComponents;
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
	};

	struct CameraComponent
	{
		Ref<Camera> camera = CreateRef<Camera>();
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
		Animation animation;
	};
}