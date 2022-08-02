#include "EditorCamera.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Time.h"

void Snow::EditorCamera::UpdateMovement()
{
	static POINT prevMousePos = InputManager::GetMousePosition();
	POINT currMousePos = InputManager::GetMousePosition();

	if (myViewportWindowSelected && InputManager::IsKeyDown(Mouse::MOUSERBUTTON))
	{
		float speed = (InputManager::IsKeyDown(Key::SHIFT)) ? myMoveSpeed * 2.5f : myMoveSpeed;

		// Rotate
		float xDiff = (float)(currMousePos.x - prevMousePos.x);
		float yDiff = (float)(currMousePos.y - prevMousePos.y);

		AdjustRotation(yDiff * myRotationSpeed, xDiff * myRotationSpeed, 0.f);

		glm::vec3 clampResult = GetRotation();
		clampResult.x = glm::clamp(clampResult.x, glm::radians(-90.f), glm::radians(90.f));
		SetRotation(clampResult);

		glm::mat4 camMat = glm::transpose(myViewMatrix);

		// Forward & Backwards
		if (InputManager::IsKeyDown(Key::W))
		{
			AdjustPosition(speed * glm::vec3(camMat[2]) * Time::GetDeltaTime());

		}
		if (InputManager::IsKeyDown(Key::S))
		{
			AdjustPosition(speed * -glm::vec3(camMat[2]) * Time::GetDeltaTime());

		}

		// Right & Left
		if (InputManager::IsKeyDown(Key::D))
		{
			AdjustPosition(speed * glm::vec3(camMat[0]) * Time::GetDeltaTime());
		}
		if (InputManager::IsKeyDown(Key::A))
		{
			AdjustPosition(speed * -glm::vec3(camMat[0]) * Time::GetDeltaTime());
		}

		// Up & down
		if (InputManager::IsKeyDown(Key::E) || InputManager::IsKeyDown(Key::SPACE))
		{
			AdjustPosition(0.f, speed * Time::GetDeltaTime(), 0.f);
		}
		if (InputManager::IsKeyDown(Key::Q) || InputManager::IsKeyDown(Key::CONTROL))
		{
			AdjustPosition(0.f, -speed * Time::GetDeltaTime(), 0.f);
		}
	}
	
	prevMousePos = currMousePos;
}

