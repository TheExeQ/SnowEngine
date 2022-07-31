#include "WindowContainer.h"
#include "Engine.h"
#include "Core/DX11.h"
#include "Editor/SceneViewportPanel.h"
#include <iostream>

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
				std::cout << "Failed to resize swapchain buffers" << std::endl;
				return;
			}

			DX11::Get().CreateRenderTargetView();
			DX11::Get().CreateDepthStencil();
			
			SceneViewportPanel::Get().OnResize();

			std::cout << "Window successfully resized" << std::endl;
		}
	}

	LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (::ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		switch (uMsg)
		{
		case WM_SIZE:
		{
			int width = LOWORD(lParam);  // Macro to get the low-order word.
			int height = HIWORD(lParam); // Macro to get the high-order word.

			// Respond to the message:
			Engine::GetWindowContainer()->OnResize((UINT)wParam, width, height);
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

		auto error = GetLastError();
		if (error)
		{
			PrintCSBackupAPIErrorMessage(error);
			return false;
		}

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
			auto error = GetLastError();
			PrintCSBackupAPIErrorMessage(error);
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
		std::cout << "Successfully initialized Win32 window!" << std::endl;
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

	// Display error message text, given an error code.
	// Typically, the parameter passed to this function is retrieved
	// from GetLastError().
	void PrintCSBackupAPIErrorMessage(DWORD dwErr)
	{

		WCHAR   wszMsgBuff[512];  // Buffer for text.

		DWORD   dwChars;  // Number of chars returned.

		// Try to get the message from the system errors.
		dwChars = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			wszMsgBuff,
			512,
			NULL);

		if (0 == dwChars)
		{
			// The error code did not exist in the system errors.
			// Try Ntdsbmsg.dll for the error code.

			HINSTANCE hInst;

			// Load the library.
			hInst = LoadLibrary(L"Ntdsbmsg.dll");
			if (NULL == hInst)
			{
				printf("cannot load Ntdsbmsg.dll\n");
				exit(1);  // Could 'return' instead of 'exit'.
			}

			// Try getting message text from ntdsbmsg.
			dwChars = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				hInst,
				dwErr,
				0,
				wszMsgBuff,
				512,
				NULL);

			// Free the library.
			FreeLibrary(hInst);

		}

		// Display the error message, or generic text if not found.
		printf("Error value: %d Message: %ws\n",
			dwErr,
			dwChars ? wszMsgBuff : L"Error message not found.");
	}
}