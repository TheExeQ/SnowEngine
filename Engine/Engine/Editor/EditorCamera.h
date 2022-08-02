#pragma once
#include "Engine/Renderer/Camera.h"

namespace Snow
{
	class EditorCamera : public Camera
	{
	public:
		void UpdateMovement();
		
	private:
		float myMoveSpeed = 1.f;
		float myRotationSpeed = 0.1f;
	};
}
