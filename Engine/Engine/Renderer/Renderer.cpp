#include "Renderer.h"
#include "Engine/Engine.h"
#include <iostream>

namespace Snow
{
	bool Renderer::Initialize(HWND hwnd)
	{
		if (!myGraphicsAPI.Initialize(hwnd)) { return false; }
		if (!CreateShaders()) { return false; }
		if (!CreateConstantBuffers()) { return false; }

		return true;
	}

	bool Renderer::CreateShaders()
	{
		//Create the input assembler
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
		hr = myFrameBuffer.Init(DX11::Device.Get(), DX11::Context.Get());
		if (FAILED(hr))
		{
			std::cout << "Failed to create constant buffer" << std::endl;
			return false;
		}

		hr = myFrameBuffer.ApplyChanges();
		if (FAILED(hr))
		{
			std::cout << "Failed to update constant buffer" << std::endl;
			return false;
		}

		hr = myObjectBuffer.Init(DX11::Device.Get(), DX11::Context.Get());
		if (FAILED(hr))
		{
			std::cout << "Failed to create constant buffer" << std::endl;
			return false;
		}

		hr = myObjectBuffer.ApplyChanges();
		if (FAILED(hr))
		{
			std::cout << "Failed to update constant buffer" << std::endl;
			return false;
		}
		return true;
	}

	void Renderer::BeginFrame()
	{
		const float bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		const float sceneBgColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

		DX11::Context->OMSetRenderTargets(1, DX11::Get().myRenderTargetView.GetAddressOf(), DX11::Get().myDepthStencilView.Get());
		DX11::Context->ClearRenderTargetView(DX11::Get().myRenderTargetView.Get(), (Engine::Get().GetRunMode() == EngineRunMode::Editor) ? bgColor : sceneBgColor);
		DX11::Context->ClearDepthStencilView(DX11::Get().myDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		DX11::Context->OMSetDepthStencilState(DX11::Get().myDepthStencilState.Get(), 1);

		if (Engine::Get().GetRunMode() == EngineRunMode::Editor)
		{
			DX11::Context->OMSetRenderTargets(1, DX11::Get().myEditorRenderTargetView.GetAddressOf(), DX11::Get().myEditorDepthStencilView.Get());
			DX11::Context->ClearRenderTargetView(DX11::Get().myEditorRenderTargetView.Get(), sceneBgColor);
			DX11::Context->ClearDepthStencilView(DX11::Get().myEditorDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			DX11::Context->OMSetDepthStencilState(DX11::Get().myEditorDepthStencilState.Get(), 1);
		}

		DX11::Context->RSSetState(DX11::Get().myRasterizerState.Get());
		DX11::Context->PSSetSamplers(0, 1, DX11::Get().mySamplerState.GetAddressOf());

		auto objectsToRender = Engine::GetActiveScene()->RenderScene(myMainCamera);
		if (myMainCamera)
		{
			for (const auto& object : objectsToRender)
			{
				auto objMatrix = object.first->GetTransform();

				myFrameBuffer.myData.ViewMatrix = myMainCamera->GetViewMatrix();
				myFrameBuffer.myData.ProjectionMatrix = myMainCamera->GetProjectionMatrix();
				myFrameBuffer.ApplyChanges();

				myObjectBuffer.myData.WorldMatrix = objMatrix;
				myObjectBuffer.ApplyChanges();

				DX11::Context->PSSetShaderResources(0, 1, object.second->material.myAlbedo.myTextureView.GetAddressOf());
				for (auto mesh : object.second->model.myMeshes)
				{
					DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					DX11::Context->IASetInputLayout(myVertexShader.GetInputLayout());

					DX11::Context->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
					DX11::Context->PSSetShader(myPixelShader.GetShader(), nullptr, 0);

					DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
					DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

					UINT offset = 0;

					DX11::Context->IASetVertexBuffers(0, 1, mesh.myVertexBuffer.GetAddressOf(), mesh.myVertexBuffer.StridePtr(), &offset);
					DX11::Context->IASetIndexBuffer(mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
					DX11::Context->DrawIndexed(mesh.myIndexBuffer.BufferSize(), 0, 0);
				}
			}
		}

		if (Engine::Get().GetRunMode() == EngineRunMode::Editor)
		{
			DX11::Context->OMSetRenderTargets(1, DX11::Get().myRenderTargetView.GetAddressOf(), DX11::Get().myDepthStencilView.Get());
			DX11::Context->ClearRenderTargetView(DX11::Get().myRenderTargetView.Get(), bgColor);
		}
	}

	void Renderer::EndFrame()
	{
		DX11::SwapChain->Present((UINT)myVSyncEnabled, NULL);
	}
}