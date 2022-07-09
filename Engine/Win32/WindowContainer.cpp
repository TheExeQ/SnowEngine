#include "WindowContainer.h"
#include <iostream>

int WindowContainer::myWindowWidth = 0;
int WindowContainer::myWindowHeight = 0;

void OnResize(HWND hwnd, UINT flag, int width, int height)
{
	// Handle resizing
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
		int width = LOWORD(lParam);  // Macro to get the low-order word.
		int height = HIWORD(lParam); // Macro to get the high-order word.

		// Respond to the message:
		OnResize(hwnd, (UINT)wParam, width, height);
	}
	break;
	}
	DefWindowProc(hwnd, uMsg, wParam, lParam);
	return true;
}

bool WindowContainer::Initialize(HINSTANCE ahInstance, const int& aWidth, const int& aHeight,std::string aWindowTitle, const std::string& aWindowClass)
{
	myHInstance = ahInstance;
	
	myWindowTitle = aWindowTitle;
	myWindowTitleWide = std::wstring(aWindowTitle.begin(), aWindowTitle.end());
	myWindowClass = aWindowClass;
	myWindowClassWide = std::wstring(aWindowClass.begin(), aWindowClass.end());
	
	myWindowWidth = aWidth;
	myWindowHeight = aHeight;

	// Register the window class.
	WNDCLASS wc = { };

	wc.lpfnWndProc = WinProc;
	wc.hInstance = myHInstance;
	wc.lpszClassName = myWindowClassWide.c_str();

	RegisterClass(&wc);

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

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

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

	ShowWindow(hwnd, SW_SHOW);
	std::cout << "Successfully initialized Win32 window!" << std::endl;
	return true;
}

bool WindowContainer::ProcessMessages()
{
	MSG msg = { };
	
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
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
			return false;
		}
	}

	return true;
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