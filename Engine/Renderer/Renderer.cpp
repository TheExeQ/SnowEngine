#include "Renderer.h"
#include <iostream>

bool Renderer::Initialize(HWND hwnd, const int& aWidth, const int& aHeight)
{
	if (!myGraphicsAPI.Initialize(hwnd, aWidth, aHeight)) { return false; }
	if (!CreateShaders()) { return false; }
	if (!CreateConstantBuffers()) { return false; }
	if (!CreateCube()) { return false; }
	return true;
}

bool Renderer::CreateCube()
{
	HRESULT hr;

	Vertex vertices[] =
	{
		Vertex(-0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f), //Left Bottom
		Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f), //Left Top
		Vertex(0.5f, 0.5f, -0.5f, 0.f, 0.f, 1.f), //Right Top
		Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.f, 0.f), //Right Bottom
		Vertex(-0.5f, -0.5f, 0.5f, 1.f, 1.f, 0.f), //Left Bottom
		Vertex(-0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.f), //Left Top
		Vertex(0.5f, 0.5f, 0.5f, 1.f, 0.f, 1.f), //Right Top
		Vertex(0.5f, -0.5f, 0.5f, 1.f, 1.f, 1.f), //Right Bottom
	};

	hr = myVertexBuffer.Initialize(DX11::Device.Get(), vertices, ARRAYSIZE(vertices));
	if (FAILED(hr))
	{
		std::cout << "Failed to initialize vertex buffer" << std::endl;
		return false;
	}

	DWORD indicies[] =
	{
		//Front
		0, 1, 2,
		0, 2, 3,

		// Back
		6, 5, 4,
		7, 6, 4,

		// Left
		0, 4, 5,
		0, 5, 1,

		// Right
		2, 6, 7,
		2, 7, 3,

		// Top
		1, 5, 6,
		1, 6, 2,

		// Bottom
		4, 0, 3,
		4, 3, 7,
	};

	hr = myIndexBuffer.Initialize(DX11::Device.Get(), indicies, ARRAYSIZE(indicies));
	if (FAILED(hr))
	{
		std::cout << "Failed to initialize index buffer" << std::endl;
		return false;
	}

	myMainCamera.SetProjectionValues(90.f, 16.f / 9.f, 0.1f, 1000.f);
	myMainCamera.SetPosition(0.f, 0.f, -2.f);

	myFrameBuffer.myData.ViewProjectionMatrix = glm::mat4(1.f);
	myFrameBuffer.myData.ViewProjectionMatrix = myMainCamera.GetProjectionMatrix() * myMainCamera.GetViewMatrix() * myFrameBuffer.myData.ViewProjectionMatrix;
	myFrameBuffer.myData.ViewProjectionMatrix = glm::transpose(myFrameBuffer.myData.ViewProjectionMatrix);
	myFrameBuffer.Update();
	return true;
}

bool Renderer::CreateShaders()
{
	//Create the input assembler
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (!myVertexShader.Init(L"../bin/Shaders/Default-vs.cso", inputLayoutDesc, ARRAYSIZE(inputLayoutDesc)))
	{
		return false;
	};

	if (!myPixelShader.Init(L"../bin/Shaders/Default-ps.cso"))
	{
		return false;
	};
	return true;
}

bool Renderer::CreateConstantBuffers()
{
	HRESULT hr;
	// Set transform matrix of the sqaure to constant buffer
	hr = myFrameBuffer.Init(DX11::Device.Get(), DX11::Context.Get());
	if (FAILED(hr))
	{
		std::cout << "Failed to create constant buffer" << std::endl;
		return false;
	}

	hr = myFrameBuffer.Update();
	if (FAILED(hr))
	{
		std::cout << "Failed to update constant buffer" << std::endl;
		return false;
	}
	return true;
}

void Renderer::BeginFrame()
{
	static auto obj = glm::mat4(1.f);
	obj = glm::rotate(obj, glm::radians(0.5f), { 0.f, 1.f, 0.f });
	myFrameBuffer.myData.ViewProjectionMatrix = myMainCamera.GetProjectionMatrix() * myMainCamera.GetViewMatrix() * obj;
	myFrameBuffer.myData.ViewProjectionMatrix = glm::transpose(myFrameBuffer.myData.ViewProjectionMatrix);
	myFrameBuffer.Update();

	const float bgColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

	DX11::Context->OMSetRenderTargets(1, DX11::Get().myRenderTargetView.GetAddressOf(), DX11::Get().myDepthStencilView.Get());
	DX11::Context->ClearRenderTargetView(DX11::Get().myRenderTargetView.Get(), bgColor);
	DX11::Context->ClearDepthStencilView(DX11::Get().myDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DX11::Context->OMSetDepthStencilState(DX11::Get().myDepthStencilState.Get(), 1);
	DX11::Context->RSSetState(DX11::Get().myRasterizerState.Get());

	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(myVertexShader.GetInputLayout());

	DX11::Context->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
	DX11::Context->PSSetShader(myPixelShader.GetShader(), nullptr, 0);

	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	UINT offset = 0;

	DX11::Context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.StridePtr(), &offset);
	DX11::Context->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DX11::Context->DrawIndexed(myIndexBuffer.BufferSize(), 0, 0);
}

void Renderer::EndFrame()
{
	DX11::SwapChain->Present((UINT)myVSyncEnabled, NULL);
}