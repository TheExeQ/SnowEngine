#include "Camera.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Scene.h"

namespace Snow
{
	Camera::Camera()
	{
		SetProjectionValues(90.f, 16.f / 9.f, 0.1f, 1000.f);
		myPosVector = glm::vec4(0.f, 0.f, 0.f, 0.f);
		myRotVector = glm::vec4(0.f, 0.f, 0.f, 0.f);
		UpdateViewMatrix();
	}

	Camera::Camera(const Camera& aCamera)
	{
		myPosVector = aCamera.myPosVector;
		myRotVector = aCamera.myRotVector;
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

	const glm::mat4& Camera::GetViewMatrix() const
	{
		return myViewMatrix;
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		return myProjectionMatrix;
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return myPosVector;
	}

	const glm::vec3& Camera::GetRotation() const
	{
		return myRotVector;
	}

	void Camera::SetIsPrimary(bool aChoise)
	{
		auto cameraEntities = Engine::GetActiveScene()->myRegistry.view<CameraComponent>();
		for (auto entity : cameraEntities)
		{
			Entity camEntity(entity, Engine::GetActiveScene());
			camEntity.GetComponent<CameraComponent>()->camera.myIsPrimary = false;
		}
		
		myIsPrimary = aChoise;
	}

	void Camera::SetPosition(const glm::vec3& pos)
	{
		myPosVector = glm::vec4(pos, 0.f);
		UpdateViewMatrix();
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		myPosVector = glm::vec4(x, y, z, 0.f);
		UpdateViewMatrix();
	}

	void Camera::AdjustPosition(const glm::vec3& pos)
	{
		myPosVector += glm::vec4(pos, 0.f);
		UpdateViewMatrix();
	}

	void Camera::AdjustPosition(float x, float y, float z)
	{
		myPosVector += glm::vec4(x, y, z, 0.f);
		UpdateViewMatrix();
	}

	void Camera::SetRotation(const glm::vec3& rot)
	{
		myRotVector = glm::vec4(rot, 0.f);
		UpdateViewMatrix();
	}

	void Camera::SetRotation(float x, float y, float z)
	{
		myRotVector = glm::vec4(x, y, z, 0.f);
		UpdateViewMatrix();
	}

	void Camera::AdjustRotation(const glm::vec3& rot)
	{
		myRotVector += glm::vec4(rot, 0.f);
		UpdateViewMatrix();
	}

	void Camera::AdjustRotation(float x, float y, float z)
	{
		myRotVector += glm::vec4(x, y, z, 0.f);
		UpdateViewMatrix();
	}

	void Camera::SetLookAtPos(glm::vec3 lookAtPos)
	{
		//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
		if (lookAtPos.x == myPosVector.x && lookAtPos.y == myPosVector.y && lookAtPos.z == myPosVector.z)
			return;

		lookAtPos.x = myPosVector.x - lookAtPos.x;
		lookAtPos.y = myPosVector.y - lookAtPos.y;
		lookAtPos.z = myPosVector.z - lookAtPos.z;

		float pitch = 0.0f;
		if (lookAtPos.y != 0.0f)
		{
			const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
			pitch = atan(lookAtPos.y / distance);
		}

		float yaw = 0.0f;
		if (lookAtPos.x != 0.0f)
		{
			yaw = atan(lookAtPos.x / lookAtPos.z);
		}
		if (lookAtPos.z > 0)
			yaw += glm::pi<float>();

		SetRotation(pitch, yaw, 0.0f);
	}

	void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
	{
		//Calculate camera rotation matrix
		glm::mat4 camRotationMatrix = glm::toMat4(glm::quat(glm::vec3(myRotVector)));
		
		//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
		glm::vec4 camTarget = camRotationMatrix * DEFAULT_FORWARD_VECTOR_GLM;
		
		//Adjust cam target to be offset by the camera's current position
		camTarget += myPosVector;
		
		//Calculate up direction based on current rotation
		glm::vec4 up = camRotationMatrix * DEFAULT_UP_VECTOR_GLM;

		//Rebuild view matrix
		myViewMatrix = glm::lookAtLH(glm::vec3(myPosVector), glm::vec3(camTarget), glm::vec3(up));
	}
}