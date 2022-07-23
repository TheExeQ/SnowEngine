#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"

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
	bool Initialize(HWND hwnd, const int& aWidth, const int& aHeight);
	void BeginFrame();
	void EndFrame();
	
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
	bool CreateDepthStencil();
	bool CreateRasterizer();
	bool CreateShaders();
	bool CreateConstantBuffers();
	bool CreateCube();

	VertexShader myVertexShader;
	PixelShader myPixelShader;
	VertexBuffer<Vertex> myVertexBuffer;
	IndexBuffer myIndexBuffer;
	ConstantBuffer<FrameBuffer> myFrameBuffer;

	ComPtr<ID3D11RenderTargetView> myRenderTargetView;
	ComPtr<ID3D11DepthStencilView> myDepthStencilView;
	ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> myDepthStencilState;
	ComPtr<ID3D11RasterizerState> myRasterizerState;

	UINT myWindowWidth = 0;
	UINT myWindowHeight = 0;

	Camera myMainCamera;

	inline static DX11* myInstance = nullptr;
};

