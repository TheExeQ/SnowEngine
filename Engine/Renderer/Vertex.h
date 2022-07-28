#pragma once

struct Vertex
{
	Vertex() {};
	Vertex(float x, float y, float z, float r, float g, float b)
	{
		this->x = x;
		this->y = y;
		this->z = z;

		this->r = r;
		this->g = g;
		this->b = b;
	}
	float x = 0.f, y = 0.f, z = 0.f;
	float r = 1.f, g = 1.f, b = 1.f;
};