#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Renderer/Color.h"

using namespace Microsoft::WRL;

class Texture
{
public:
	Texture() { LoadTexture(&Colors::MissingTextureColor, 1, 1); };
	void LoadTexture(const Color* colorData, UINT width, UINT height);
	void LoadTexture(const char* aFileName);
	
private:
	friend class Renderer;
	bool LoadTextureFromFile(const char* filename);

	ComPtr<ID3D11Resource> myTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView> myTextureView = nullptr;
	float myWidth = 0.0f;
	float myHeight = 0.0f;
};

