#include "Mesh.h"
#include "Core/DX11.h"
#include <iostream>

namespace Snow
{
	Mesh::Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices)
	{
		HRESULT hr;

		hr = myVertexBuffer.Initialize(DX11::Device.Get(), aVertices.data(), aVertices.size());
		if (FAILED(hr))
		{
			std::cout << "Failed to initialize vertex buffer" << std::endl;
		}

		hr = myIndexBuffer.Initialize(DX11::Device.Get(), aIndices.data(), aIndices.size());
		if (FAILED(hr))
		{
			std::cout << "Failed to initialize index buffer" << std::endl;
		}
	}

	Mesh::Mesh(const Mesh& aMesh)
	{
		myVertexBuffer = aMesh.myVertexBuffer;
		myIndexBuffer = aMesh.myIndexBuffer;
	}
}