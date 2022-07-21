#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <DirectXMath.h>
using namespace DirectX;


class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	const glm::vec4& GetPositionVector() const;
	const glm::vec3& GetPositionFloat3() const;
	const glm::vec4& GetRotationVector() const;
	const glm::vec3& GetRotationFloat3() const;

	void SetPosition(const glm::vec4& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const glm::vec4& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const glm::vec4& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const glm::vec4& rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(glm::vec3 lookAtPos);

private:
	void UpdateViewMatrix();
	glm::vec4 posVector;
	glm::vec4 rotVector;
	glm::vec3 pos;
	glm::vec3 rot;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const glm::vec4 DEFAULT_FORWARD_VECTOR_GLM = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	const glm::vec4 DEFAULT_UP_VECTOR_GLM = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
};