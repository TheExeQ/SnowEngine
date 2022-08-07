#pragma once
#include <array>
#include <glm/glm.hpp>

#define MAX_AMOUNT_OF_BONES 128

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
		std::array<glm::mat4, MAX_AMOUNT_OF_BONES> BoneTransforms;
	};
}