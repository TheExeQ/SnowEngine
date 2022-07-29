#include "Texture.h"
#include "Core/DX11.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

void Texture::LoadTexture(const char* aFileName)
{
	LoadTextureFromFile(aFileName);
	myFilePath = aFileName;
}

void Texture::LoadTexture(const Color* colorData, UINT width, UINT height)
{
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(Color);
	
	HRESULT hr = DX11::Device->CreateTexture2D(&textureDesc, &initialData, &pTexture);
	if (FAILED(hr))
	{
		std::cout << "Failed to initialize texture from color data." << std::endl;
	}
	myTexture = static_cast<ID3D11Texture2D*>(pTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	
	hr = DX11::Device->CreateShaderResourceView(myTexture.Get(), &srvDesc, myTextureView.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "Failed to create shader resource view from texture generated from color data." << std::endl;
	}
}

bool Texture::LoadTextureFromFile(const char* filename)
{
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
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
		std::cout << "Failed to create shader resource view from texture generated from " << filename << std::endl;
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
	hr = DX11::Device->CreateShaderResourceView(pTexture, &srvDesc, myTextureView.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "Failed to create shader resource view from texture generated from " << filename << std::endl;
	}

	pTexture->Release();

	DX11::Context->GenerateMips(myTextureView.Get());

	myWidth = image_width;
	myHeight = image_height;
	stbi_image_free(image_data);
	return true;
}