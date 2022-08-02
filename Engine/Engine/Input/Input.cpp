#include "Input.h"

namespace Snow
{
	InputManager::InputManager()
		:
		myCurrentMousePos{ 0, 0 }
	{
		if (myInstance == nullptr)
		{
			myInstance = this;
		}
	}

	POINT InputManager::GetMousePosition()
	{
		return myInstance->myCurrentMousePos;
	}

	bool InputManager::IsKeyPressed(unsigned char aKeyCode)
	{
		if (myInstance->myKeystates[aKeyCode])
		{
			myInstance->myKeystates[aKeyCode] = false;
			return true;
		}
		else
		{
			return myInstance->myKeystates[aKeyCode];
		}
	}

	bool InputManager::IsKeyDown(unsigned char aKeyCode)
	{
		return myInstance->myKeystates[aKeyCode] || myInstance->myKeystatesHold[aKeyCode];
	}

	void InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (!(lParam & (1 << 30)))
			{
				myInstance->OnKeyPressed(wParam);
			}
			else
			{
				myInstance->myKeystates[wParam] = false;
				myInstance->OnKeyHold(wParam);
			}
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			myInstance->OnKeyReleased(wParam);
			break;

		case WM_MOUSEMOVE:
			GetCursorPos(&myInstance->myCurrentMousePos);
			break;

		case WM_LBUTTONDOWN:
			myInstance->OnKeyPressed(0x01);
			break;

		case WM_LBUTTONUP:
			myInstance->OnKeyReleased(0x01);
			break;

		case WM_RBUTTONDOWN:
			myInstance->OnKeyPressed(0x02);
			break;

		case WM_RBUTTONUP:
			myInstance->OnKeyReleased(0x02);
			break;

		case WM_MBUTTONDOWN:
			myInstance->OnKeyPressed(0x04);
			break;

		case WM_MBUTTONUP:
			myInstance->OnKeyReleased(0x04);
			break;

		case WM_KILLFOCUS:
			myInstance->ClearState();
			break;

		default:
			break;
		}
	}

	void InputManager::OnKeyPressed(unsigned char aKeyCode)
	{
		myKeystates[aKeyCode] = true;
	}

	void InputManager::OnKeyHold(unsigned char aKeyCode)
	{
		myKeystatesHold[aKeyCode] = true;
	}

	void InputManager::OnKeyReleased(unsigned char aKeyCode)
	{
		myKeystates[aKeyCode] = false;
		myKeystatesHold[aKeyCode] = false;
	}

	void InputManager::ClearState()
	{
		myKeystates.reset();
		myKeystatesHold.reset();
	}
}
