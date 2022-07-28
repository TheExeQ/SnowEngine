#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class DX11
{
public:
	static DX11& Get() { return *myInstance; };

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;
	
private:
	friend class Renderer;
	bool Initialize(HWND hwnd, const int& aWidth, const int& aHeight);
	
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
	bool CreateDepthStencil();
	bool CreateRasterizer();
	bool CreateSamplerState();

	ComPtr<ID3D11RenderTargetView> myRenderTargetView;
	ComPtr<ID3D11DepthStencilView> myDepthStencilView;
	ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> myDepthStencilState;
	ComPtr<ID3D11RasterizerState> myRasterizerState;
	ComPtr<ID3D11SamplerState> mySamplerState;

	UINT myWindowWidth = 0;
	UINT myWindowHeight = 0;

	inline static DX11* myInstance = nullptr;
};

