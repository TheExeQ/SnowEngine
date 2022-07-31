#pragma once
#include <glm/glm.hpp>

namespace Snow
{
	struct FrameBuffer
	{
		glm::mat4 ViewProjectionMatrix;
	};

	struct ObjectBuffer
	{
		glm::mat4 WorldMatrix;
	};
}