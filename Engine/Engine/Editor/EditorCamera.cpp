#include "EditorCamera.h"
#include "Engine/Engine.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Time.h"

#include "Engine/Editor/SceneViewportPanel.h"
#include <ImGuizmo/ImGuizmo.h>

void Snow::EditorCamera::UpdateMovement(glm::vec3& outPosition, glm::vec3& outRotation)
{
	if (Engine::GetActiveScene()->GetSceneState() == SceneState::Play) { return; }
	
	static POINT prevMousePos = InputManager::GetMousePosition();
	POINT currMousePos = InputManager::GetMousePosition();

	if (myViewportWindowSelected && InputManager::IsKeyDown(Mouse::MOUSERBUTTON))
	{
		SceneViewportPanel::Get().myShowGizmos = false;
		float speed = (InputManager::IsKeyDown(Key::SHIFT)) ? myMoveSpeed * 2.5f : myMoveSpeed;

		// Rotate
		float xDiff = (float)(currMousePos.x - prevMousePos.x);
		float yDiff = (float)(currMousePos.y - prevMousePos.y);

		outRotation += glm::vec3(yDiff * myRotationSpeed, xDiff * myRotationSpeed, 0.f);

		glm::vec3 clampResult = outRotation;
		clampResult.x = glm::clamp(clampResult.x, glm::radians(-90.f), glm::radians(90.f));
		outRotation = clampResult;

		glm::mat4 camMat = glm::transpose(myViewMatrix);

		// Forward & Backwards
		if (InputManager::IsKeyDown(Key::W))
		{
			outPosition += (speed * glm::vec3(camMat[2]) * Time::GetDeltaTime());

		}
		if (InputManager::IsKeyDown(Key::S))
		{
			outPosition += (speed * -glm::vec3(camMat[2]) * Time::GetDeltaTime());

		}

		// Right & Left
		if (InputManager::IsKeyDown(Key::D))
		{
			outPosition += (speed * glm::vec3(camMat[0]) * Time::GetDeltaTime());
		}
		if (InputManager::IsKeyDown(Key::A))
		{
			outPosition += (speed * -glm::vec3(camMat[0]) * Time::GetDeltaTime());
		}

		// Up & down
		if (InputManager::IsKeyDown(Key::E) || InputManager::IsKeyDown(Key::SPACE))
		{
			outPosition += glm::vec3(0.f, speed * Time::GetDeltaTime(), 0.f);
		}
		if (InputManager::IsKeyDown(Key::Q) || InputManager::IsKeyDown(Key::CONTROL))
		{
			outPosition += glm::vec3(0.f, -speed * Time::GetDeltaTime(), 0.f);
		}
	}
	else
	{
		SceneViewportPanel::Get().myShowGizmos = true;
		if (InputManager::IsKeyPressed(Key::W)) // Set Translate
		{
			SceneViewportPanel::Get().myGizmoOperation = ImGuizmo::TRANSLATE;
		}
		if (InputManager::IsKeyPressed(Key::E)) // Set Rotate
		{
			SceneViewportPanel::Get().myGizmoOperation = ImGuizmo::ROTATE;
		}
		if (InputManager::IsKeyPressed(Key::R)) // Set Scale
		{
			SceneViewportPanel::Get().myGizmoOperation = ImGuizmo::SCALE;
		}
	}
	
	UpdateViewMatrix(outPosition, outRotation);

	prevMousePos = currMousePos;
}

