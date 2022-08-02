#pragma once
#include <array>
#include <Windows.h>
#include <queue>
#include <bitset>

#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace Snow
{
	class InputManager
	{
	public:

		// Mouse events
		static POINT GetMousePosition();

		// Key events
		static bool IsKeyPressed(unsigned char aKeyCode);
		static bool IsKeyDown(unsigned char aKeyCode);

		static void UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		friend class WindowContainer;
		InputManager();
		
		void OnKeyPressed(unsigned char aKeyCode);
		void OnKeyHold(unsigned char aKeyCode);
		void OnKeyReleased(unsigned char aKeyCode);
		void ClearState();

	private:
		inline static InputManager* myInstance = nullptr;

		static constexpr unsigned int myNKeys = 256u;
		static constexpr unsigned int myBufferSize = 16u;
		std::bitset<myNKeys> myKeystates;
		std::bitset<myNKeys> myKeystatesHold;

		POINT myCurrentMousePos;
	};
}
