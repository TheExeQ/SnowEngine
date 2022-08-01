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
		class Event
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};
		private:
			Type myType;
			unsigned char myCode;
		public:
			Event(Type aType, unsigned char aCode)
				:
				myType(aType),
				myCode(aCode)
			{}
			bool IsPress() const
			{
				return myType == Type::Press;
			}
			bool IsRelease() const
			{
				return myType == Type::Release;
			}
			unsigned char GetCode() const
			{
				return myCode;
			}
		};

		InputManager();
		static InputManager* Get();

		// Mouse events
		POINT GetMousePosition();

		// Key events
		bool IsKeyPressed(unsigned char aKeyCode);
		bool IsKeyDown(unsigned char aKeyCode) const;
		Event ReadKey();
		bool KeyIsEmpty() const;
		void FlushKey();

		// Char events
		Event ReadChar();
		bool CharIsEmpty() const;
		void FlushChar();
		void Flush();

		void UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void OnKeyPressed(unsigned char aKeyCode);
		void OnKeyHold(unsigned char aKeyCode);
		void OnKeyReleased(unsigned char aKeyCode);
		void OnChar(char aChar);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& aBuffer);

	private:
		inline static InputManager* myInstance = nullptr;

		static constexpr unsigned int myNKeys = 256u;
		static constexpr unsigned int myBufferSize = 16u;
		std::bitset<myNKeys> myKeystates;
		std::bitset<myNKeys> myKeystatesHold;
		std::queue<Event> myKeybuffer;
		std::queue<Event> myCharbuffer;

		POINT myCurrentMousePos;
	};
}
