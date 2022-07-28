#pragma once
#include "Renderer/Camera.h"
#include "Renderer/Model.h"
#include "Renderer/Material.h"

#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec3 position = {0.f, 0.f, 0.f};
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
