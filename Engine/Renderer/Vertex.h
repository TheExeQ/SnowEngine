#pragma once

struct Vertex
{
	Vertex(float x, float y, float z, float r, float g, float b)
	{
		this->x = x;
		this->y = y;
		this->z = z;

		this->r = r;
		this->g = g;
		this->b = b;
	}
	float x, y, z;
	float r, g, b;
};