#include "Renderer.h"
#include "MeshFactory.h"
#include "Engine.h"
#include <iostream>

#include "Scene/Entity.h"

bool Renderer::Initialize(HWND hwnd, const int& aWidth, const int& aHeight)
{
	if (!myGraphicsAPI.Initialize(hwnd, aWidth, aHeight)) { return false; }
	if (!CreateShaders()) { return false; }
	if (!CreateConstantBuffers()) { return false; }
	if (!InitScene()) { return false; }
	return true;
}

bool Renderer::InitScene()
{
	auto cube = Engine::Get().GetCurrentScene().CreateEntity();
	auto pyramid = Engine::Get().GetCurrentScene().CreateEntity();

	cube.GetComponent<TransformComponent>().position = { -1.0f, 0.0f, 0.0f };
	cube.GetComponent<TransformComponent>().rotation = {45.f, 45.0f, 0.0f };
	cube.AddComponent<StaticMeshComponent>().mesh = MeshFactory::CreateCube();

	pyramid.GetComponent<TransformComponent>().position = { 1.0f, 0.0f, 0.0f };
	pyramid.GetComponent<TransformComponent>().rotation = { 0.0f, 0.0f, 0.0f };
	pyramid.AddComponent<StaticMeshComponent>().mesh = MeshFactory::CreatePyramid();

	myCube = MeshFactory::CreateCube();

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
	auto objectsToRender = Engine::Get().GetCurrentScene().RenderScene(myMainCamera);

	const float bgColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

	DX11::Context->OMSetRenderTargets(1, DX11::Get().myRenderTargetView.GetAddressOf(), DX11::Get().myDepthStencilView.Get());
	DX11::Context->ClearRenderTargetView(DX11::Get().myRenderTargetView.Get(), bgColor);
	DX11::Context->ClearDepthStencilView(DX11::Get().myDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DX11::Context->OMSetDepthStencilState(DX11::Get().myDepthStencilState.Get(), 1);
	DX11::Context->RSSetState(DX11::Get().myRasterizerState.Get());

	for (const auto& object : objectsToRender)
	{
		auto objMatrix = glm::mat4(1.f);
		objMatrix = glm::translate(objMatrix, object.first->position);
		objMatrix = glm::rotate(objMatrix, glm::radians(object.first->rotation.x), { 1.f, 0.f, 0.f });
		objMatrix = glm::rotate(objMatrix, glm::radians(object.first->rotation.y), { 0.f, 1.f, 0.f });
		objMatrix = glm::rotate(objMatrix, glm::radians(object.first->rotation.z), { 0.f, 0.f, 1.f });
		objMatrix = glm::scale(objMatrix, object.first->scale);

		myFrameBuffer.myData.ViewProjectionMatrix = myMainCamera.GetProjectionMatrix() * myMainCamera.GetViewMatrix() * objMatrix;
		myFrameBuffer.myData.ViewProjectionMatrix = glm::transpose(myFrameBuffer.myData.ViewProjectionMatrix);
		myFrameBuffer.Update();

		DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11::Context->IASetInputLayout(myVertexShader.GetInputLayout());

		DX11::Context->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
		DX11::Context->PSSetShader(myPixelShader.GetShader(), nullptr, 0);

		DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

		UINT offset = 0;

		DX11::Context->IASetVertexBuffers(0, 1, object.second->mesh.myVertexBuffer.GetAddressOf(), object.second->mesh.myVertexBuffer.StridePtr(), &offset);
		DX11::Context->IASetIndexBuffer(object.second->mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DX11::Context->DrawIndexed(object.second->mesh.myIndexBuffer.BufferSize(), 0, 0);
	}
}

void Renderer::EndFrame()
{
	DX11::SwapChain->Present((UINT)myVSyncEnabled, NULL);
}