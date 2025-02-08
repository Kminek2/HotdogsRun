#pragma once
#include "Transform.h"
struct CameraTransform :
    public Transform
{
	glm::vec2 rotation;

	/// <summary>
	/// Set rotation to the given vector in degrees
	/// </summary>
	/// <param name="rotation">glm::vec2(yaw, pitch)</param>
	void RotateTo(glm::vec2 rotation) {
		this->rotation = ModuloRotation(rotation);
		UpdateVectors();
	}

	/// <summary>
	/// Set rotation to the given vector in degrees
	/// </summary>
	void RotateTo(float yaw = 0, float pitch = 0) {
		RotateTo(glm::vec2(yaw, pitch));
	}
	
	/// <summary>
	/// Rotates the camera by the given amount on each axis, in degrees
	/// </summary>
	/// <param name="rotation">glm::vec2(yaw, pitch)</param>
	void Rotate(glm::vec2 by) {
		this->rotation = ModuloRotation(this->rotation - by);
		UpdateVectors();
	}

	/// <summary>
	/// Rotates the camera by the given amount on each axis, in degrees
	/// </summary>
	void Rotate(float yaw = 0, float pitch = 0) {
		Rotate(glm::vec2(yaw, pitch));
	}

	CameraTransform(glm::vec3 pos = glm::vec3(0), glm::vec2 rot = glm::vec2(0), glm::vec3 scale = glm::vec3(1)) : Transform(pos) {
		position = pos;
		rotation = rot;
		this->scale = scale;

		UpdateVectors();
	}

private:
	/// <summary>
	/// Takes in a rotation vector in degrees, and clamps each angle to [0, 360)
	/// </summary>
	/// <returns>(glm::vec3) -> (glm::vec3)</returns>
	glm::vec2 ModuloRotation(glm::vec2 rotation) {
		rotation = glm::vec2(ClampRotation(rotation.x), glm::clamp(rotation.y, -89.0f, 89.0f));
		return rotation;
	}

	/// <summary>
	/// Takes in an angle in degrees, and returns the angle clamped to [0, 360)
	/// </summary>
	/// <returns>(float) -> (float)</returns>
	float ClampRotation(float r) {
		while (r >= 360.0f) r -= 360.0f;
		while (r < 0.0f) r += 360.0f;
		return r;
	}

	void UpdateVectors() {
		glm::vec3 direction;
		direction.x = cos(glm::radians(this->rotation.x)) * cos(glm::radians(this->rotation.y));
		direction.z = sin(glm::radians(this->rotation.y));
		direction.y = sin(glm::radians(this->rotation.x)) * cos(glm::radians(this->rotation.y));
		front = glm::normalize(direction);
		glm::vec3 upDir = glm::vec3(0, 0, 1);

		right = glm::cross(front, upDir);

		up = -glm::cross(front, right);
	}
	friend class GameObject;
};

