#pragma once

namespace Snow
{
	class Mesh;

	class MeshFactory
	{
	public:
		static Mesh CreateCube();
		static Mesh CreatePyramid();
	};
}