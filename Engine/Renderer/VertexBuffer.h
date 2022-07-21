#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<typename T>
class VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(const VertexBuffer<T>& rhs);

	VertexBuffer<T>& operator=(const VertexBuffer<T>& rhs);

	inline ID3D11Buffer* Get()
	{
		return myBuffer.Get();
	}

	inline ID3D11Buffer* const* GetAddressOf()
	{
		return myBuffer.GetAddressOf();
	}

	inline UINT BufferSize()
	{
		return myBufferSize;
	}

	inline const UINT Stride()
	{
		return *myStride.get();
	}

	inline const UINT* StridePtr()
	{
		return myStride.get();
	}

	inline HRESULT Initialize(ID3D11Device* aDevice, T* data, UINT numVertices)
	{
		if (myBuffer.Get())
		{
			myBuffer.Reset();
		}
		if (!myStride)
		{
			myStride = std::make_unique<UINT>(sizeof(T));
		}

		myBufferSize = numVertices;

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		HRESULT hr = aDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, myBuffer.GetAddressOf());
		return hr;
	}

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	std::shared_ptr<UINT> myStride;
	UINT myBufferSize;
};

template<typename T>
VertexBuffer<T>& VertexBuffer<T>::operator=(const VertexBuffer<T>& rhs)
{
	myBuffer = rhs.myBuffer;
	myBufferSize = rhs.myBufferSize;
	myStride = rhs.myStride;
	return *this;
}

template<typename T>
VertexBuffer<T>::VertexBuffer(const VertexBuffer<T>& rhs)
{
	myBuffer = rhs.myBuffer;
	myBufferSize = rhs.myBufferSize;
	myStride = rhs.myStride;
}