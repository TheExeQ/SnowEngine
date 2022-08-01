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

	InputManager* InputManager::Get()
	{
		return myInstance;
	}

	POINT InputManager::GetMousePosition()
	{
		return myCurrentMousePos;
	}

	InputManager::Event InputManager::ReadKey()
	{
		// TODO: Change to return optional
		InputManager::Event nullEvent(Event::Type::Invalid, '0');
		if (myKeybuffer.size() > 0u)
		{
			InputManager::Event e = myKeybuffer.front();
			myKeybuffer.pop();
			return e;
		}
		return nullEvent;
	}

	InputManager::Event InputManager::ReadChar()
	{
		// TODO: Change to return optional
		InputManager::Event nullEvent(Event::Type::Invalid, '0');
		if (myCharbuffer.size() > 0u)
		{
			InputManager::Event e = myCharbuffer.front();
			myCharbuffer.pop();
			return e;
		}
		return nullEvent;
	}

	bool InputManager::IsKeyPressed(unsigned char aKeyCode)
	{
		if (myKeystates[aKeyCode])
		{
			myKeystates[aKeyCode] = false;
			return true;
		}
		else
		{
			return myKeystates[aKeyCode];
		}
	}

	bool InputManager::IsKeyDown(unsigned char aKeyCode) const
	{
		return myKeystates[aKeyCode] || myKeystatesHold[aKeyCode];
	}

	bool InputManager::KeyIsEmpty() const
	{
		return myKeybuffer.empty();
	}

	bool InputManager::CharIsEmpty() const
	{
		return myCharbuffer.empty();
	}

	void InputManager::FlushKey()
	{
		myKeybuffer = std::queue<Event>();
	}

	void InputManager::FlushChar()
	{
		myCharbuffer = std::queue<Event>();
	}

	void InputManager::Flush()
	{
		FlushKey();
		FlushChar();
	}

	void InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (!(lParam & (1 << 30)))
			{
				OnKeyPressed(wParam);
			}
			else
			{
				myKeystates[wParam] = false;
				OnKeyHold(wParam);
			}
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			OnKeyReleased(wParam);
			break;

		case WM_CHAR:
			OnChar(wParam);
			break;

		case WM_MOUSEMOVE:
			GetCursorPos(&myCurrentMousePos);
			break;

		case WM_LBUTTONDOWN:
			OnKeyPressed(0x01);
			break;

		case WM_LBUTTONUP:
			OnKeyReleased(0x01);
			break;

		case WM_RBUTTONDOWN:
			OnKeyPressed(0x02);
			break;

		case WM_RBUTTONUP:
			OnKeyReleased(0x02);
			break;

		case WM_MBUTTONDOWN:
			OnKeyPressed(0x04);
			break;

		case WM_MBUTTONUP:
			OnKeyReleased(0x04);
			break;

		case WM_KILLFOCUS:
			ClearState();
			break;

		default:
			break;
		}
	}

	void InputManager::OnKeyPressed(unsigned char aKeyCode)
	{
		myKeystates[aKeyCode] = true;
		myKeybuffer.push(InputManager::Event(InputManager::Event::Type::Press, aKeyCode));
		TrimBuffer(myKeybuffer);
	}

	void InputManager::OnKeyHold(unsigned char aKeyCode)
	{
		myKeystatesHold[aKeyCode] = true;
	}

	void InputManager::OnKeyReleased(unsigned char aKeyCode)
	{
		myKeystates[aKeyCode] = false;
		myKeystatesHold[aKeyCode] = false;
		myKeybuffer.push(InputManager::Event(InputManager::Event::Type::Release, aKeyCode));
		TrimBuffer(myKeybuffer);
	}

	void InputManager::OnChar(char aChar)
	{
		myCharbuffer.push(InputManager::Event(InputManager::Event::Type::Press, aChar));
		TrimBuffer(myCharbuffer);
	}

	void InputManager::ClearState()
	{
		myKeystates.reset();
		myKeystatesHold.reset();
	}

	template<typename T>
	void InputManager::TrimBuffer(std::queue<T>& aBuffer)
	{
		while (aBuffer.size() > myBufferSize)
		{
			aBuffer.pop();
		}
	}
}
