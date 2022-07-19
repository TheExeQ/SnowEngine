#include "DX11.h"
#include "Win32/WindowContainer.h"
#include <iostream>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

ComPtr<ID3D11Device> DX11::Device = nullptr;
ComPtr<ID3D11DeviceContext> DX11::Context = nullptr;
ComPtr<IDXGISwapChain> DX11::SwapChain = nullptr;

bool DX11::Initialize(HWND hwnd)
{
	if (!myInstance) { myInstance = this; }
	else { return false; }

	if (!CreateDeviceAndSwapChain(hwnd)) { return false; };
	if (!CreateRenderTargetView()) { return false; };
	if (!CreateDepthStencil()) { return false; };
	if (!CreateRasterizer()) { return false; };
	if (!CreateShaders()) { return false; };
	if (!CreateConstantBuffers()) { return false; };
	if (!SetupImGui(hwnd)) { return false; };

	std::cout << "Successfully initialized DirectX!" << std::endl;
	return true;
}

bool DX11::CleanUp()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return true;
}

bool DX11::CreateDeviceAndSwapChain(HWND hwnd)
{
	HRESULT result;

	RECT windowRect;

	if (!GetWindowRect(hwnd, &windowRect))
	{
		std::cout << "Failed to get window rect!" << std::endl;
	}

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// Settings for SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
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
	depthStencilBufferDesc.Width = WindowContainer::GetWidth();
	depthStencilBufferDesc.Height = WindowContainer::GetHeight();
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
		std::cout << "Failed to create depth stencil view" << std::endl;
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
	vp.Width = WindowContainer::GetWidth();
	vp.Height = WindowContainer::GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &vp);

	return true;
}

bool DX11::CreateShaders()
{
	// Create the input assembler
	//D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};

	//if (!myVertexShader.Init(L"../bin/Default-vs.cso", inputLayoutDesc, ARRAYSIZE(inputLayoutDesc)))
	//{
	//	return false;
	//};

	//if (!myPixelShader.Init(L"../bin/Default-ps.cso"))
	//{
	//	return false;
	//};
	return true;
}

bool DX11::CreateConstantBuffers()
{
	//HRESULT hr;
	//// Set transform matrix of the sqaure to constant buffer
	//hr = myConstantBuffer.Init(myDevice.Get(), myContext.Get());
	//if (FAILED(hr))
	//{
	//	std::cout << "Failed to create constant buffer" << std::endl;
	//	return false;
	//}

	//hr = myConstantBuffer.Update();
	//if (FAILED(hr))
	//{
	//	std::cout << "Failed to update constant buffer" << std::endl;
	//	return false;
	//}
	return true;
}

bool DX11::SetupImGui(HWND hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Device.Get(), Context.Get());

	return true;
}

bool DX11::RenderFrame()
{
	const float bgColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

	Context->OMSetRenderTargets(1, myRenderTargetView.GetAddressOf(), myDepthStencilView.Get());
	Context->ClearRenderTargetView(myRenderTargetView.Get(), bgColor);
	Context->ClearDepthStencilView(myDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Context->OMSetDepthStencilState(myDepthStencilState.Get(), 1);
	Context->RSSetState(myRasterizerState.Get());

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	SwapChain->Present(1, NULL);
	return true;
}
