#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

using namespace Microsoft::WRL;

class VertexShader
{
public:
	bool Init(std::wstring aFilePath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aDescSize);
	ID3D11VertexShader* GetShader();
	ID3D11InputLayout* GetInputLayout();
	ID3DBlob* GetBuffer();
	
private:
	ComPtr<ID3D11VertexShader> myShader;
	ComPtr<ID3D11InputLayout> myInputLayout;
	ComPtr<ID3DBlob> myShaderBuffer;
};

class PixelShader
{
public:
	bool Init(std::wstring aFilePath);
	ID3D11PixelShader* GetShader();
	ID3DBlob* GetBuffer();
	
private:
	ComPtr<ID3D11PixelShader> myShader;
	ComPtr<ID3DBlob> myShaderBuffer;
};