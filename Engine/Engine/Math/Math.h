#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/matrix4x4.h>
#include <ozz/base/maths/simd_math.h>

namespace Snow
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
		glm::mat4 ConvertAssimpMat4ToGlmMat4(const aiMatrix4x4& aMatrix);
		glm::mat4 ConvertOzzMat4ToGlmMat4(const ozz::math::Float4x4& aMatrix);
	}
}