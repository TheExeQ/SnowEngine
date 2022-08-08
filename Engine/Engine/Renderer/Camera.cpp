#include "Camera.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Scene.h"

namespace Snow
{
	Camera::Camera()
	{
		SetProjectionValues(90.f, 16.f / 9.f, 0.1f, 1000.f);
		UpdateViewMatrix(glm::vec3(0.f), glm::vec3(0.f));
	}

	Camera::Camera(const Camera& aCamera)
	{
		myViewMatrix = aCamera.myViewMatrix;
		myProjectionMatrix = aCamera.myProjectionMatrix;
	}

	void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
	{
		myProjectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearZ, farZ);

		myFov = fovDegrees;
		myNearPlane = nearZ;
		myFarPlane = farZ;
	}

	const glm::mat4& Camera::GetViewMatrix(glm::vec3 aPosition, glm::vec3 aRotation)
	{
		UpdateViewMatrix(aPosition, aRotation);
		return myViewMatrix;
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		return myProjectionMatrix;
	}

	void Camera::SetIsPrimary(bool aChoise)
	{
		auto cameraEditorEntities = Engine::GetEditorScene()->myRegistry.view<CameraComponent>();
		for (auto entity : cameraEditorEntities)
		{
			Engine::GetEditorScene()->myRegistry.get<CameraComponent>(entity).camera->myIsPrimary = false;
		}

		auto cameraEntities = Engine::GetActiveScene()->myRegistry.view<CameraComponent>();
		for (auto entity : cameraEntities)
		{
			Engine::GetActiveScene()->myRegistry.get<CameraComponent>(entity).camera->myIsPrimary = false;
		}
		
		myIsPrimary = aChoise;
	}

	void Camera::UpdateViewMatrix(glm::vec3 aPosition, glm::vec3 aRotation) //Updates view matrix and also updates the movement vectors
	{
		//Calculate camera rotation matrix
		glm::mat4 camRotationMatrix = glm::toMat4(glm::quat(aRotation));
		
		//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
		glm::vec4 camTarget = camRotationMatrix * DEFAULT_FORWARD_VECTOR_GLM;
		
		//Adjust cam target to be offset by the camera's current position
		camTarget += glm::vec4(aPosition, 1.f);
		
		//Calculate up direction based on current rotation
		glm::vec4 up = camRotationMatrix * DEFAULT_UP_VECTOR_GLM;

		//Rebuild view matrix
		myViewMatrix = glm::lookAtLH(aPosition, glm::vec3(camTarget), glm::vec3(up));
	}
}