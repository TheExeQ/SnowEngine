#pragma once
#include "Engine/Input/Input.h"
#include <windows.h>
#include <string>

namespace Snow
{
	class WindowContainer
	{
	public:
		WindowContainer() = default;
		~WindowContainer() = default;

		HWND GetWindowHandle() const { return myHandle; };

		void OnResize(UINT flag, int width, int height);

		static int GetWindowWidth() { return myWindowWidth; };
		static int GetClientWidth() { return myWindowWidth; };
		static int GetWindowHeight() { return myWindowHeight; };
		static int GetClientHeight() { return myWindowHeight; };

	private:
		friend class Engine;
		bool Initialize(HINSTANCE ahInstance, const int& aWidth, const int& aHeight,
			std::string aWindowTitle, const std::string& aWindowClass);
		void ProcessMessages();

		HWND myHandle;
		HINSTANCE myHInstance;
		std::string myWindowTitle;
		std::wstring myWindowTitleWide;
		std::string myWindowClass;
		std::wstring myWindowClassWide;

		InputManager myInputManager;

		inline static int myWindowWidth = 0;
		inline static int myClientWidth = 0;
		inline static int myWindowHeight = 0;
		inline static int myClientHeight = 0;
	};
}