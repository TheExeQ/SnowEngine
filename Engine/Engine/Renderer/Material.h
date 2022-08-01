#pragma once
#include "Engine/Renderer/Texture.h"

namespace Snow
{
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
		friend class SceneSerializer;
		Texture myAlbedo;
		Texture myRoughnessMap;
		Texture myMetalnessMap;
		Texture myNormalMap;
	};
}