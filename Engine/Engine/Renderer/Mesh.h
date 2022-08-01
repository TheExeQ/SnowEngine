#pragma once
#include "VertexBuffer.h"
#include "Vertex.h"
#include "IndexBuffer.h"

namespace Snow
{
	class Mesh
	{
	public:
		Mesh() {};
		Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);
		Mesh(const Mesh& aMesh);

	private:
		friend class Renderer;

		VertexBuffer<Vertex> myVertexBuffer;
		IndexBuffer myIndexBuffer;
	};
}