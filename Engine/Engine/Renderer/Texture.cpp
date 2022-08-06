#include "Texture.h"
#include "Engine/Core/DX11.h"
#include "Engine/Debug/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Snow
{
	bool Texture::LoadTexture(const char* aFilepath)
	{
		auto it = myTextures.find(std::string(aFilepath));
		if (it != myTextures.end())
		{
			myWidth = it->second->myWidth;
			myHeight = it->second->myHeight;
			myFilePath = it->second->myFilePath;
			myTexture = it->second->myTexture;
			myTextureView = it->second->myTextureView;
			CORE_LOG_INFO("Texture reused.");
			return true;
		}
		
		LoadTextureFromFile(aFilepath);
		myFilePath = aFilepath;
		myTextures[std::string(aFilepath)] = CreateRef<Texture>(*this);
	}

	bool Texture::LoadTexture(const Color* colorData, UINT width, UINT height)
	{
		auto name = std::string(std::to_string(colorData->GetR()) + std::to_string(colorData->GetG()) + std::to_string(colorData->GetB()));
		auto it = myTextures.find(name);
		if (it != myTextures.end())
		{
			myWidth = it->second->myWidth;
			myHeight = it->second->myHeight;
			myFilePath = it->second->myFilePath;
			myTexture = it->second->myTexture;
			myTextureView = it->second->myTextureView;
			CORE_LOG_INFO("Texture reused!");
			return true;
		}

		CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
		ID3D11Texture2D* pTexture = nullptr;
		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = colorData;
		initialData.SysMemPitch = width * sizeof(Color);

		HRESULT hr = DX11::Device->CreateTexture2D(&textureDesc, &initialData, &pTexture);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to initialize texture from color data.");
		}

		myTexture = static_cast<ID3D11Texture2D*>(pTexture);
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);

		hr = DX11::Device->CreateShaderResourceView(myTexture.Get(), &srvDesc, &myTextureView);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to create shader resource view from texture generated from color data.");
		}

		pTexture->Release();
		myTextures[std::string(name)] = CreateRef<Texture>(*this);
	}

	bool Texture::LoadTextureFromFile(const char* aFilepath)
	{
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(aFilepath, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 0;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Texture2D* pTexture = NULL;

		HRESULT hr = DX11::Device->CreateTexture2D(&desc, nullptr, &pTexture);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR(std::string("Failed to create shader resource view from texture generated from ") + std::string(aFilepath));
		}
		myTexture = static_cast<ID3D11Texture2D*>(pTexture);

		DX11::Context->UpdateSubresource(pTexture, 0u, nullptr, image_data, desc.Width * 4, 0u);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension =
			D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = -1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = DX11::Device->CreateShaderResourceView(pTexture, &srvDesc, &myTextureView);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR(std::string("Failed to create shader resource view from texture generated from ") + std::string(aFilepath));
		}

		pTexture->Release();

		DX11::Context->GenerateMips(myTextureView.Get());

		myWidth = image_width;
		myHeight = image_height;
		stbi_image_free(image_data);
		return true;
	}
}