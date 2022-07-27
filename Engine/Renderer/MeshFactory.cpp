#include "MeshFactory.h"
#include "Mesh.h"
#include <vector>

Mesh MeshFactory::CreateCube()
{
	std::vector<Vertex> vertices =
	{
		// Front
		Vertex(-0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f), //Left Bottom
		Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f), //Left Top
		Vertex(0.5f, 0.5f, -0.5f, 0.f, 0.f, 1.f), //Right Top
		Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.f, 0.f), //Right Bottom
		
		// Back
		Vertex(-0.5f, -0.5f, 0.5f, 1.f, 1.f, 0.f), //Left Bottom
		Vertex(-0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.f), //Left Top
		Vertex(0.5f, 0.5f, 0.5f, 1.f, 0.f, 1.f), //Right Top
		Vertex(0.5f, -0.5f, 0.5f, 1.f, 1.f, 1.f), //Right Bottom
	};

	std::vector<DWORD> indicies =
	{
		//Front
		0, 1, 2,
		0, 2, 3,

		// Back
		6, 5, 4,
		7, 6, 4,

		// Left
		0, 4, 5,
		0, 5, 1,

		// Right
		2, 6, 7,
		2, 7, 3,

		// Top
		1, 5, 6,
		1, 6, 2,

		// Bottom
		4, 0, 3,
		4, 3, 7,
	};

	return Mesh(vertices, indicies);
}

Mesh MeshFactory::CreatePyramid()
{
	std::vector<Vertex> vertices =
	{
		Vertex(0.f, 0.5f, 0.f, 0.f, 0.f, 0.f), //Top
		
		// Front
		Vertex(-0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f), //Left Bottom
		Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.f, 0.f), //Right Bottom
		
		// Back
		Vertex(-0.5f, -0.5f, 0.5f, 1.f, 1.f, 0.f), //Left Bottom
		Vertex(0.5f, -0.5f, 0.5f, 1.f, 1.f, 1.f), //Right Bottom
	};

	std::vector<DWORD> indicies =
	{
		// Front
		1, 0, 2,
		
		// Right
		2, 0, 4,

		// Back
		4, 0, 3,

		// Left
		3, 0, 1,

		// Bottom
		4, 3, 1,
		2, 4, 1,
	};

	return Mesh(vertices, indicies);
}
