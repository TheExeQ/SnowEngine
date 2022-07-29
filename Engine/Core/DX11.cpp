#include "DX11.h"
#include "Win32/WindowContainer.h"
#include <iostream>

ComPtr<ID3D11Device> DX11::Device = nullptr;
ComPtr<ID3D11DeviceContext> DX11::Context = nullptr;
ComPtr<IDXGISwapChain> DX11::SwapChain = nullptr;

bool DX11::Initialize(HWND hwnd, const int& aWidth, const int& aHeight)
{
	if (!myInstance) { myInstance = this; }
	else { return false; }

	RECT windowRect;

	GetClientRect(hwnd, &windowRect);

	myWindowWidth = windowRect.right - windowRect.left;
	myWindowHeight = windowRect.bottom - windowRect.top;

	if (!CreateDeviceAndSwapChain(hwnd)) { return false; };
	if (!CreateRenderTargetView()) { return false; };
	if (!CreateDepthStencil()) { return false; };
	if (!CreateRasterizer()) { return false; };
	if (!CreateSamplerState()) { return false; };

	std::cout << "Successfully initialized DirectX!" << std::endl;
	return true;
}

bool DX11::CreateDeviceAndSwapChain(HWND hwnd)
{
	HRESULT result;

	// Settings for SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = myWindowWidth;
	swapChainDesc.BufferDesc.Height = myWindowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create the SwapChain
	result =
		D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&SwapChain,
			&Device,
			NULL,
			&Context
		);

	if (FAILED(result))
	{
		std::cout << "Failed to create device and swap chain" << std::endl;
		return false;
	}
	return true;
}

bool DX11::CreateRenderTargetView()
{
	HRESULT result;

	// Get the back buffer from the swap chain
	ID3D11Texture2D* backBuffer;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to get back buffer" << std::endl;
		return false;
	}

	// Create the render target view with the back buffer
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;

	result = Device->CreateRenderTargetView(backBuffer, &rtvd, myRenderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		std::cout << "Failed to create render target view" << std::endl;
		return false;
	}

	backBuffer->Release();
	return true;
}

bool DX11::CreateDepthStencil()
{
	HRESULT result;

	// Create the depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
	depthStencilBufferDesc.Width = myWindowWidth;
	depthStencilBufferDesc.Height = myWindowHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	result = Device->CreateTexture2D(&depthStencilBufferDesc, NULL, myDepthStencilBuffer.GetAddressOf());
	if (FAILED(result))
	{
		std::cout << "Failed to create depth stencil texture" << std::endl;
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = depthStencilBufferDesc.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	result = Device->CreateDepthStencilView(myDepthStencilBuffer.Get(), &dsvd, myDepthStencilView.GetAddressOf());
	if (FAILED(result))
	{
		std::cout << "Failed to create depth stencil view" << std::endl;
		return false;
	}

	// Create the depth state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;

	result = Device->CreateDepthStencilState(&depthStencilDesc, myDepthStencilState.GetAddressOf());
	if (FAILED(result))
	{
		std::cout << "Failed to create depth stencil state" << std::endl;
		return false;
	}
	return true;
}

bool DX11::CreateRasterizer()
{
	// Create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = Device->CreateRasterizerState(&rasterizerDesc, myRasterizerState.GetAddressOf());
	if (FAILED(result))
	{
		std::cout << "Failed to create rasterizer state" << std::endl;
		return false;
	}

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)myWindowWidth;
	vp.Height = (FLOAT)myWindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &vp);

	return true;
}

bool DX11::CreateSamplerState()
{
	HRESULT hr;
	
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = Device->CreateSamplerState(&sampDesc, mySamplerState.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "Failed to create sampler state." << std::endl;
		return false;
	}
	return true;
}