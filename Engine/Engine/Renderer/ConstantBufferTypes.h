#pragma once
#include <array>
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
		glm::mat4 WorldTransform;
		std::array<glm::mat4, 128> BoneTransforms;
	};
}