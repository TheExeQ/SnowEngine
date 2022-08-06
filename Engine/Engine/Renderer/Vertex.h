#pragma once
#include <glm/glm.hpp>

#define MAX_NUM_BONES_PER_VERTEX 4

namespace Snow
{
	struct Vertex
	{
		Vertex() {};
		Vertex(glm::vec3 aPosition, glm::vec2 aTexCoords)
		{
			position = aPosition;
			texCoords = aTexCoords;
		};
		Vertex(float x, float y, float z, float u, float v)
		{
			this->position.x = x;
			this->position.y = y;
			this->position.z = z;

			this->texCoords.x = u;
			this->texCoords.y = v;
		}
		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec2 texCoords = { 0.f, 0.f };

		uint32_t boneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
		float weights[MAX_NUM_BONES_PER_VERTEX] = { 0.f };
	};
}