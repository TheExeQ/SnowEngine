#include "WindowContainer.h"
#include "Engine/Engine.h"
#include "Engine/Core/DX11.h"
#include "Engine/Editor/SceneViewportPanel.h"
#include "Engine/Debug/Log.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Snow
{
	void WindowContainer::OnResize(UINT flag, int width, int height)
	{
		// Handle resizing
		
		RECT windowRect;

		myWindowWidth = width;
		myWindowHeight = height;

		GetClientRect(myHandle, &windowRect);

		myClientWidth = windowRect.right - windowRect.left;
		myClientHeight = windowRect.bottom - windowRect.top;

		if (&DX11::Get())
		{

			DX11::Context->OMSetRenderTargets(0, 0, 0);

			DX11::Get().myRenderTargetView->Release();
			DX11::Get().myDepthStencilView->Release();
			DX11::Get().myDepthStencilState->Release();
			DX11::Get().myDepthStencilTexture->Release();

			HRESULT hr;
			hr = DX11::SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
			if (FAILED(hr))
			{
				CORE_LOG_ERROR("Failed to resize swapchain buffers");
				return;
			}

			DX11::Get().CreateRenderTargetView();
			DX11::Get().CreateDepthStencil();
			
			if (Engine::Get().GetRunMode() == EngineRunMode::Editor)
			{
				SceneViewportPanel::Get().OnResize();
			}

			CORE_LOG_INFO("Window successfully resized");
		}
	}

	LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (::ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		InputManager::UpdateEvents(uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_SIZE:
		{
			int width = LOWORD(lParam);  // Macro to get the low-order word.
			int height = HIWORD(lParam); // Macro to get the high-order word.

			if (width != 0 && height != 0)
			{
				// Respond to the message:
				Engine::GetWindowContainer()->OnResize((UINT)wParam, width, height);
			}
			break;
		}

		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
		{
			Engine::Get().myIsRunning = false;
			break;
		}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	bool WindowContainer::Initialize(HINSTANCE ahInstance, const int& aWidth, const int& aHeight, std::string aWindowTitle, const std::string& aWindowClass)
	{
		myHInstance = ahInstance;

		myWindowTitle = aWindowTitle;
		myWindowTitleWide = std::wstring(aWindowTitle.begin(), aWindowTitle.end());
		myWindowClass = aWindowClass;
		myWindowClassWide = std::wstring(aWindowClass.begin(), aWindowClass.end());

		// Register the window class.
		WNDCLASSEX wcx = { };
		wcx.cbSize = sizeof(wcx);
		wcx.lpfnWndProc = WinProc;
		wcx.hInstance = myHInstance;
		wcx.lpszClassName = myWindowClassWide.c_str();

		RegisterClassEx(&wcx);

		// Create the window.

		HWND hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			myWindowClassWide.c_str(),      // Window class
			myWindowTitleWide.c_str(),		// Window text
			WS_OVERLAPPEDWINDOW,            // Window style

			// Position & Size
			CW_USEDEFAULT, CW_USEDEFAULT, aWidth, aHeight,

			NULL,       // Parent window    
			NULL,       // Menu
			myHInstance, // Instance handle
			NULL        // Additional application data
		);

		if (hwnd == NULL)
		{
			return false;
		}

		myHandle = hwnd;

		myWindowWidth = aWidth;
		myWindowHeight = aHeight;

		RECT windowRect;

		GetClientRect(hwnd, &windowRect);

		myClientWidth = windowRect.right - windowRect.left;
		myClientHeight = windowRect.bottom - windowRect.top;

		ShowWindow(hwnd, SW_SHOW);
		CORE_LOG_INFO("Successfully initialized Win32 window!");
		return true;
	}

	void WindowContainer::ProcessMessages()
	{
		MSG msg = { };

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_NULL)
		{
			if (!IsWindow(myHandle))
			{
				myHandle = NULL;
				UnregisterClass(myWindowClassWide.c_str(), myHInstance);
			}
		}
	}
}