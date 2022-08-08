#pragma once
#include "Engine/Renderer/Camera.h"

namespace Snow
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() { myIsPrimary = true; };
		void UpdateMovement(glm::vec3& outPosition, glm::vec3& outRotation);
		
	private:
		friend class SceneViewportPanel;

		inline static bool myViewportWindowSelected = false;

		float myMoveSpeed = 1.f;
		float myRotationSpeed = 0.01f;
	};
}
