#pragma once
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Material.h"

#include <glm/glm.hpp>

namespace Snow
{
	struct TagComponent
	{
		std::string name;
	};

	struct TransformComponent
	{
		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec3 rotation = { 0.f, 0.f, 0.f };
		glm::vec3 scale = { 1.f, 1.f, 1.f };
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
}