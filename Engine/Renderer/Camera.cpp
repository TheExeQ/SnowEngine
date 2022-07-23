#include "Camera.h"

Camera::Camera()
{
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	this->posVector = glm::vec4(pos, 0.f);
	this->rot = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotVector = glm::vec4(rot, 0.f);
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * glm::two_pi<float>();
	this->projectionMatrix = glm::perspective(fovRadians, aspectRatio, nearZ, farZ);
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const glm::vec4& Camera::GetPositionVector() const
{
	return this->posVector;
}

const glm::vec3& Camera::GetPositionFloat3() const
{
	return this->pos;
}

const glm::vec4& Camera::GetRotationVector() const
{
	return this->rotVector;
}

const glm::vec3& Camera::GetRotationFloat3() const
{
	return this->rot;
}

void Camera::SetPosition(const glm::vec4& pos)
{
	this->pos = glm::vec3(pos.x, pos.y, pos.z);
	this->posVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = glm::vec3(x, y, z);
	this->posVector = glm::vec4(pos, 0.f);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const glm::vec4& pos)
{
	this->posVector += pos;
	this->pos = glm::vec3(pos.x, pos.y, pos.z);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = glm::vec4(pos, 0.f);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const glm::vec4& rot)
{
	this->rotVector = rot;
	this->rot = glm::vec3(rot.x, rot.y, rot.z);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = glm::vec3(x, y, z);
	this->rotVector = glm::vec4(rot, 0.f);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const glm::vec4& rot)
{
	this->rotVector += rot;
	this->rot = glm::vec3(rotVector.x, rotVector.y, rotVector.z);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = glm::vec4(rot, 0.f);
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(glm::vec3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

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

	this->SetRotation(pitch, yaw, 0.0f);
}

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	glm::mat4 camRotationMatrix2 = glm::eulerAngleXYZ(this->rot.x, this->rot.y, this->rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	glm::vec4 camTarget2 = camRotationMatrix2 * DEFAULT_FORWARD_VECTOR_GLM;
	//Adjust cam target to be offset by the camera's current position
	camTarget2 += posVector;
	//Calculate up direction based on current rotation
	glm::vec4 upDir2 = camRotationMatrix2 * DEFAULT_UP_VECTOR_GLM;

	glm::vec3 camTarg = glm::vec3(camTarget2.x, camTarget2.y, camTarget2.z);
	glm::vec3 up = glm::vec3(upDir2.x, upDir2.y, upDir2.z);

	//Rebuild view matrix
	this->viewMatrix = glm::lookAtLH(pos, camTarg, up);
}