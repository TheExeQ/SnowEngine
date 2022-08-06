#include "Mesh.h"
#include "Engine/Core/DX11.h"
#include "Engine/Debug/Log.h"

namespace Snow
{
	void Mesh::Init(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices)
	{
		HRESULT hr;

		hr = myVertexBuffer.Initialize(DX11::Device.Get(), aVertices.data(), aVertices.size());
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to initialize vertex buffer");
		}

		hr = myIndexBuffer.Initialize(DX11::Device.Get(), aIndices.data(), aIndices.size());
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to initialize index buffer");
		}
	}

	Mesh::Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices)
	{
		Init(aVertices, aIndices);
	}

	Mesh::Mesh(const Mesh& aMesh)
	{
		myVertexBuffer = aMesh.myVertexBuffer;
		myIndexBuffer = aMesh.myIndexBuffer;
	}

	Mesh::Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices, const std::vector<Bone>& aBones)
	{
		Init(aVertices, aIndices);
		myBones = aBones;
	}
}