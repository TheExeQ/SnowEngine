#pragma once
#include <windows.h>
#include <string>

void PrintCSBackupAPIErrorMessage(DWORD dwErr);

class WindowContainer
{
public:
	WindowContainer() = default;
	~WindowContainer() = default;
	bool Initialize(HINSTANCE ahInstance, const int& aWidth, const int& aHeight, 
		std::string aWindowTitle, const std::string& aWindowClass);
	bool ProcessMessages();
	HWND GetWindowHandle() const { return myHandle; };
	static int GetWidth() { return myWindowWidth; };
	static int GetHeight() { return myWindowHeight; };

private:
	HWND myHandle;
	HINSTANCE myHInstance;
	std::string myWindowTitle;
	std::wstring myWindowTitleWide;
	std::string myWindowClass;
	std::wstring myWindowClassWide;
	static int myWindowWidth;
	static int myWindowHeight;
};

