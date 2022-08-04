#pragma comment(lib, "d3dcompiler.lib")
#include "Shaders.h"
#include <d3dcompiler.h>
#include "Engine/Debug/Log.h"
#include "Engine/Core/DX11.h"

namespace Snow
{
	bool VertexShader::Init(std::wstring aFilePath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aDescSize)
	{
		// Read file to buffer
		HRESULT hr = D3DReadFileToBlob(aFilePath.c_str(), &myShaderBuffer);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR(std::string("Failed to read file: " + std::string(aFilePath.begin(), aFilePath.end())));
			return false;
		}

		// Create vertex shader
		hr = DX11::Device->CreateVertexShader(myShaderBuffer->GetBufferPointer(), myShaderBuffer->GetBufferSize(), nullptr, &myShader);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to create vertex shader");
			return false;
		}

		// Create input layout
		hr = DX11::Device->CreateInputLayout(aDesc, aDescSize, myShaderBuffer->GetBufferPointer(), myShaderBuffer->GetBufferSize(), myInputLayout.GetAddressOf());
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to create input layout");
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
			CORE_LOG_ERROR(std::string("Failed to read file: " + std::string(aFilePath.begin(), aFilePath.end())));
			return false;
		}

		// Create pixel shader
		hr = DX11::Device->CreatePixelShader(myShaderBuffer->GetBufferPointer(), myShaderBuffer->GetBufferSize(), nullptr, &myShader);
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to create pixel shader");
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