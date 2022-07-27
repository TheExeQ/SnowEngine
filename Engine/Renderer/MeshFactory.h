#pragma once

class Mesh;

class MeshFactory
{
public:
	static Mesh CreateCube();
	static Mesh CreatePyramid();
};

