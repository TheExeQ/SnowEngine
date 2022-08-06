#pragma once
#include "VertexBuffer.h"
#include "Vertex.h"
#include "IndexBuffer.h"

#include <glm/glm.hpp>

namespace Snow
{
	struct Bone
	{
		uint32_t parentIndex;
		glm::mat4 localMatrix;
		glm::mat4 inverseModelMatrix;
	};

	class Mesh // #TODO: add submesh class so objects can be rendered in one draw call.
	{
	public:
		Mesh() {};
		Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);
		Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices, const std::vector<Bone>& aBones);
		Mesh(const Mesh& aMesh);

	protected:
		friend class Renderer;
		void Init(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);

		VertexBuffer<Vertex> myVertexBuffer;
		IndexBuffer myIndexBuffer;
		std::vector<Bone> myBones;
	};
}