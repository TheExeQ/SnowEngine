#include "EditorCamera.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Time.h"

void Snow::EditorCamera::UpdateMovement()
{
	if (InputManager::IsKeyDown(Mouse::MOUSERBUTTON))
	{
		static POINT prevMousePos = InputManager::GetMousePosition();
		POINT currMousePos = InputManager::GetMousePosition();

		// Forward & Backwards
		if (InputManager::IsKeyDown(Key::W))
		{
			AdjustPosition(0.f, 0.f, myMoveSpeed * Time::GetDeltaTime());

		}
		if (InputManager::IsKeyDown(Key::S))
		{
			AdjustPosition(0.f, 0.f, -myMoveSpeed * Time::GetDeltaTime());

		}

		// Right & Left
		if (InputManager::IsKeyDown(Key::D))
		{
			AdjustPosition(myMoveSpeed * Time::GetDeltaTime(), 0.f, 0.f);
		}
		if (InputManager::IsKeyDown(Key::A))
		{
			AdjustPosition(-myMoveSpeed * Time::GetDeltaTime(), 0.f, 0.f);
		}

		// Up & down
		if (InputManager::IsKeyDown(Key::E) || InputManager::IsKeyDown(Key::SPACE))
		{
			AdjustPosition(0.f, myMoveSpeed * Time::GetDeltaTime(), 0.f);
		}
		if (InputManager::IsKeyDown(Key::Q) || InputManager::IsKeyDown(Key::CONTROL))
		{
			AdjustPosition(0.f, -myMoveSpeed * Time::GetDeltaTime(), 0.f);
		}

		// Rotate
		float xDiff = (float)(currMousePos.x - prevMousePos.x);
		float yDiff = (float)(currMousePos.y - prevMousePos.y);
		
		AdjustRotation(yDiff * myRotationSpeed, xDiff * myRotationSpeed, 0.f);

		prevMousePos = currMousePos;
	}
}

