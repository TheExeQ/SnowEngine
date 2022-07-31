#pragma comment(lib, "d3dcompiler.lib")
#include "Shaders.h"
#include <d3dcompiler.h>
#include <iostream>
#include "Core/DX11.h"

namespace Snow
{
	bool VertexShader::Init(std::wstring aFilePath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aDescSize)
	{
		// Read file to buffer
		HRESULT hr = D3DReadFileToBlob(aFilePath.c_str(), &myShaderBuffer);
		if (FAILED(hr))
		{
			std::cout << "Failed to read file" << std::endl;
			return false;
		}

		// Create vertex shader
		hr = DX11::Device->CreateVertexShader(myShaderBuffer->GetBufferPointer(), myShaderBuffer->GetBufferSize(), nullptr, &myShader);
		if (FAILED(hr))
		{
			std::cout << "Failed to create vertex shader" << std::endl;
			return false;
		}

		// Create input layout
		hr = DX11::Device->CreateInputLayout(aDesc, aDescSize, myShaderBuffer->GetBufferPointer(), myShaderBuffer->GetBufferSize(), myInputLayout.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "Failed to create input layout" << std::endl;
			return false;
		}

		return true;
	}

	ID3D11VertexShader* VertexShader::GetShader()
	{
		return myShader.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout()
	{
		return myInputLayout.Get();
	}

	ID3DBlob* VertexShader::GetBuffer()
	{
		return myShaderBuffer.Get();
	}

	bool PixelShader::Init(std::wstring aFilePath)
	{
		// Read file to buffer
		HRESULT hr = D3DReadFileToBlob(aFilePath.c_str(), &myShaderBuffer);
		if (FAILED(hr))
		{
			std::cout << "Failed to read file" << std::endl;
			return false;
		}

		// Create pixel shader
		hr = DX11::Device->CreatePixelShader(myShaderBuffer->GetBufferPointer(), myShaderBuffer->GetBufferSize(), nullptr, &myShader);
		if (FAILED(hr))
		{
			std::cout << "Failed to create pixel shader" << std::endl;
			return false;
		}
		return true;
	}

	ID3D11PixelShader* PixelShader::GetShader()
	{
		return myShader.Get();
	}

	ID3DBlob* PixelShader::GetBuffer()
	{
		return myShaderBuffer.Get();
	}
}