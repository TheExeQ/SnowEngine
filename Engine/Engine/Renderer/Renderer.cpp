#include "Renderer.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Entity.h"

#include "Engine/Debug/Log.h"

namespace Snow
{
	bool Renderer::Initialize(HWND hwnd)
	{
		if (!myGraphicsAPI.Initialize(hwnd)) { return false; }
		if (!mySceneRenderer->Initialize()) { return false; }
		return true;
	}

	void Renderer::BeginFrame()
	{
		const float bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		const float sceneBgColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

		DX11::Context->RSSetState(DX11::Get().myRasterizerState.Get());
		DX11::Context->PSSetSamplers(0, 1, DX11::Get().mySamplerState.GetAddressOf());

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
			
			mySceneRenderer->RenderEditorScene();
			
			DX11::Context->OMSetRenderTargets(1, DX11::Get().myRenderTargetView.GetAddressOf(), DX11::Get().myDepthStencilView.Get());
			DX11::Context->ClearRenderTargetView(DX11::Get().myRenderTargetView.Get(), bgColor);
		}
		else
		{
			mySceneRenderer->RenderRuntimeScene();
		}
	}

	void Renderer::EndFrame()
	{
		DX11::SwapChain->Present((UINT)myVSyncEnabled, NULL);
	}
}