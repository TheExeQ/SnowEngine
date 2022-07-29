#pragma once
#include "Renderer/Texture.h"

class Material
{
public:
	Material();

	void SetAlbedo(const char* aFileName);
	void SetRoughness(const char* aFileName);
	void SetMetalness(const char* aFileName);
	void SetNormal(const char* aFileName);

private:
	friend class Renderer;
	friend class SceneHierarchyPanel;
	Texture myAlbedo;
	Texture myRoughnessMap;
	Texture myMetalnessMap;
	Texture myNormalMap;
};

