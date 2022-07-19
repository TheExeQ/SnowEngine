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
	friend class Engine;
	friend LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool Initialize(HWND hwnd);
	bool CleanUp();
	bool RenderFrame();
	
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
	bool CreateDepthStencil();
	bool CreateRasterizer();
	bool CreateShaders();
	bool CreateConstantBuffers();
	bool SetupImGui(HWND hwnd);

	ComPtr<ID3D11RenderTargetView> myRenderTargetView;
	ComPtr<ID3D11DepthStencilView> myDepthStencilView;
	ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> myDepthStencilState;
	ComPtr<ID3D11RasterizerState> myRasterizerState;

	inline static DX11* myInstance = nullptr;
};

