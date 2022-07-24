#pragma once
#include <glm/glm.hpp>

struct FrameBuffer
{
	glm::mat4 ViewProjectionMatrix;
};

struct ObjectBuffer
{
	glm::mat4 WorldMatrix;
};