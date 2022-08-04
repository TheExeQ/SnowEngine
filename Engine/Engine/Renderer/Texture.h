#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Color.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <unordered_map>

using namespace Microsoft::WRL;

namespace Snow
{
	class Texture // #TODO: Move myTextures map to asset class that handles loading and caching
	{
	public:
		Texture() {};
		bool LoadTexture(const char* aFilepath);
		bool LoadTexture(const Color* colorData, UINT width, UINT height);

	private:
		friend class Renderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		bool LoadTextureFromFile(const char* aFilepath);

		float myWidth = 0.0f;
		float myHeight = 0.0f;
		
		std::string myFilePath = "";
		ComPtr<ID3D11Resource> myTexture = nullptr;
		ComPtr<ID3D11ShaderResourceView> myTextureView = nullptr;

		inline static std::unordered_map<std::string, Ref<Texture>> myTextures;
	};
}
