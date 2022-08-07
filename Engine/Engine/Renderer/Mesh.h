#pragma once
#include "VertexBuffer.h"
#include "Vertex.h"
#include "IndexBuffer.h"

#include <glm/glm.hpp>

namespace Snow
{
	class Mesh // #TODO: add submesh class so objects can be rendered in one draw call.
	{
	public:
		Mesh() {};
		Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);
		Mesh(const Mesh& aMesh);

	protected:
		friend class Renderer;
		void Init(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);

		VertexBuffer<Vertex> myVertexBuffer;
		IndexBuffer myIndexBuffer;
	};
}