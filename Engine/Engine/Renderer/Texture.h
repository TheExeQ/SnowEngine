#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Engine/Renderer/Color.h"

using namespace Microsoft::WRL;

namespace Snow
{
	class Texture
	{
	public:
		Texture() {};
		void LoadTexture(const Color* colorData, UINT width, UINT height);
		void LoadTexture(const char* aFileName);

	private:
		friend class Renderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		bool LoadTextureFromFile(const char* filename);

		std::string myFilePath = "";
		ComPtr<ID3D11Resource> myTexture = nullptr;
		ComPtr<ID3D11ShaderResourceView> myTextureView = nullptr;
		float myWidth = 0.0f;
		float myHeight = 0.0f;
	};
}
