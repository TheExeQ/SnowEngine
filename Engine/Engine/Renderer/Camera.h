#pragma once

#define GLM_FORCE_LEFT_HANDED

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Snow
{
	class Camera
	{
	public:
		Camera();
		Camera(const Camera& aCamera);
		void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;

		const glm::vec4& GetPositionVec4() const;
		const glm::vec3& GetPosition() const;
		
		const glm::vec4& GetRotationVec4() const;
		const glm::vec3& GetRotation() const;

		void SetIsPrimary(bool aChoise);
		bool GetIsPrimary() const { return myIsPrimary; };
		float GetFOV() const { return myFov; };
		float GetNear() const { return myNearPlane; };
		float GetFar() const { return myFarPlane; };

		void SetPosition(const glm::vec3& pos);
		void SetPosition(float x, float y, float z);
		void AdjustPosition(const glm::vec3& pos);
		void AdjustPosition(float x, float y, float z);
		void SetRotation(const glm::vec3& rot);
		void SetRotation(float x, float y, float z);
		void AdjustRotation(const glm::vec3& rot);
		void AdjustRotation(float x, float y, float z);
		void SetLookAtPos(glm::vec3 lookAtPos);

	protected:
		friend class SceneSerializer;
		void UpdateViewMatrix();
		glm::vec4 myPosVector;
		glm::vec4 myRotVector;
		glm::mat4 myViewMatrix;
		glm::mat4 myProjectionMatrix;

		bool myIsPrimary = false;

		float myFov = 90.f;
		float myNearPlane = 0.1f;
		float myFarPlane = 1000.f;
		
		inline static const glm::vec4 DEFAULT_FORWARD_VECTOR_GLM = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		inline static const glm::vec4 DEFAULT_UP_VECTOR_GLM = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	};
}