#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace Snow
{
	class DX11
	{
	public:
		static DX11& Get() { return *myInstance; };

		static ComPtr<ID3D11Device> Device;
		static ComPtr<ID3D11DeviceContext> Context;
		static ComPtr<IDXGISwapChain> SwapChain;

	private:
		friend class Renderer;
		friend class WindowContainer;
		friend class SceneViewportPanel;
		bool Initialize(HWND hwnd);

		bool CreateDeviceAndSwapChain(HWND hwnd);
		bool CreateRenderTargetView();
		bool CreateDepthStencil();
		bool CreateEditorRenderTargetView(const int& aWidth, const int& aHeight);
		bool CreateEditorDepthStencil(const int& aWidth, const int& aHeight);
		bool CreateRasterizer();
		bool CreateSamplerState();

		ComPtr<ID3D11RenderTargetView> myRenderTargetView;
		
		ComPtr<ID3D11Texture2D> myDepthStencilTexture;
		ComPtr<ID3D11DepthStencilState> myDepthStencilState;
		ComPtr<ID3D11DepthStencilView> myDepthStencilView;

		ComPtr<ID3D11Texture2D> myEditorTexture;
		ComPtr<ID3D11RenderTargetView> myEditorRenderTargetView;
		ComPtr<ID3D11ShaderResourceView> myEditorShaderResourceView;

		ComPtr<ID3D11Texture2D> myEditorDepthStencilTexture;
		ComPtr<ID3D11DepthStencilState> myEditorDepthStencilState;
		ComPtr<ID3D11DepthStencilView> myEditorDepthStencilView;
		
		ComPtr<ID3D11RasterizerState> myRasterizerState;
		
		ComPtr<ID3D11SamplerState> mySamplerState;

		inline static DX11* myInstance = nullptr;
	};
}
