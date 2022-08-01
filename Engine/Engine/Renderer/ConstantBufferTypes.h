#pragma once
#include <glm/glm.hpp>

namespace Snow
{
	struct FrameBuffer
	{
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	};

	struct ObjectBuffer
	{
		glm::mat4 WorldMatrix;
	};
}