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

		const glm::mat4& GetViewMatrix(glm::vec3 aPosition, glm::vec3 aRotation);
		const glm::mat4& GetProjectionMatrix() const;

		void SetIsPrimary(bool aChoise);
		bool GetIsPrimary() const { return myIsPrimary; };
		float GetFOV() const { return myFov; };
		float GetNear() const { return myNearPlane; };
		float GetFar() const { return myFarPlane; };

	protected:
		friend class SceneRenderer;
		friend class SceneSerializer;
		void UpdateViewMatrix(glm::vec3 aPosition, glm::vec3 aRotation);
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