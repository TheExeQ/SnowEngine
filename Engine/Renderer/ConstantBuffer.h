#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "ConstantBufferTypes.h"

using namespace Microsoft::WRL;

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;
	HRESULT Init(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	HRESULT ApplyChanges();

	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAddressOf() const;

public:
	T myData;

private:
	ComPtr<ID3D11Buffer> myBuffer;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
};

template<typename T>
HRESULT ConstantBuffer<T>::Init(ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
{
	myDevice = aDevice;
	myContext = aContext;

	// Create constant buffer
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = myDevice->CreateBuffer(&cbDesc, NULL, &myBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

template<typename T>
HRESULT ConstantBuffer<T>::ApplyChanges()
{
	// Update constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = myContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return hr;
	}
	memcpy(mappedResource.pData, &myData, sizeof(T));
	myContext->Unmap(myBuffer.Get(), 0);
	return hr;
}

template<typename T>
ID3D11Buffer* ConstantBuffer<T>::Get() const
{
	return myBuffer.Get();
}

template<typename T>
ID3D11Buffer* const* ConstantBuffer<T>::GetAddressOf() const
{
	return myBuffer.GetAddressOf();
}
