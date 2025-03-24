#pragma once
#include "Transform.h"
#include "Application.h"
#include <mutex>

class RectTransform :
    public Transform
{
public:
	glm::vec3 position;
	float rotation;
	glm::vec2 scale;

	/// <summary>
	/// Set position to the given vector, in world space
	/// </summary>
	void MoveTo(glm::vec3 position) {
		this->position = position;
		UpdateMatrix();
	}

	/// <summary>
	/// Set rotation to the given vector in degrees
	/// </summary>
	void RotateTo(float rotation) {
		this->rotation = ModuloRotation(rotation);
		UpdateVectors();
		UpdateMatrix();
	}

	/// <summary>
	/// Set scale to the given vector
	/// </summary>
	void ScaleTo(glm::vec2 scale) {
		this->scale = scale;
		UpdateMatrix();
	}

	/// <summary>
	/// Sets the width, by default preserving image aspectRatio
	/// </summary>
	/// <param name="width">Width</param>
	/// <param name="preserveAspect">Should aspect ratio be preserved</param>
	void SetWidth(float width, bool preserveAspect = true)
	{
		if (!preserveAspect)
			scale.x = width;
		else
			scale = glm::vec2(width, (width / ASPECT_HEIGHT) * ASPECT_WIDTH);

		UpdateMatrix();
	}

	/// <summary>
	/// Sets the height, by default preserving image aspectRatio
	/// </summary>
	/// <param name="height">Heightt</param>
	/// <param name="preserveAspect">Should aspect ratio be preserved</param>
	void SetHeight(float height, bool preserveAspect = true)
	{
		if (!preserveAspect)
			scale.y = height;
		else
			scale = glm::vec2((height / ASPECT_WIDTH) * ASPECT_HEIGHT, height);

		UpdateMatrix();
	}

	/// <summary>
	/// Move the object by the given vector, in world space
	/// </summary>
	void Move(glm::vec2 by) {
		this->position += glm::vec3(by, 0);
		UpdateMatrix();
	}

	/// <summary>
	/// Rotates the object by the given amount
	/// </summary>
	void Rotate(float by) {
		this->rotation = ModuloRotation(this->rotation + by);

		UpdateMatrix();
	}

	/// <summary>
	/// Increases the scale of the object by the given amount on each axis
	/// </summary>
	void Scale(glm::vec2 by) {
		this->scale += by;
		UpdateMatrix();
	}

	/// <summary>
	/// Increases the scale of the object by the given amount on all axes
	/// </summary>
	void Scale(float by) {
		this->scale += glm::vec2(by);
		UpdateMatrix();
	}

	/// <summary>
	/// Multiplies the scale of the object by the given amount on each axis
	/// </summary>
	void ScaleTimes(glm::vec2 times) {
		this->scale *= times;
		UpdateMatrix();
	}

	/// <summary>
	/// Multiplies the scale of the object by the given amount on all axes
	/// </summary>
	void ScaleTimes(float times) {
		this->scale *= times;
		UpdateMatrix();
	}

	RectTransform(glm::vec3 pos = glm::vec3(0), float rot = 0.0f, glm::vec2 scale = glm::vec2(1)) {
		position = pos;
		rotation = rot;
		this->scale = scale * glm::vec2(ASPECT_WIDTH, ASPECT_HEIGHT) / (float)std::max(ASPECT_WIDTH, ASPECT_HEIGHT);

		UpdateMatrix();
	}


	glm::mat4 getModelMatrix() { RecalculateTransform(); std::lock_guard<std::mutex> matrixLock(matrixMutex); return modelTransform; }
private:
	std::mutex matrixMutex;
	/// <summary>
	/// Takes in a rotation float in degrees, and clamps each angle to [0, 360)
	/// </summary>
	/// <returns>(float) -> (float)</returns>
	float ModuloRotation(float rotation) {
		rotation = ClampRotation(rotation);
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

	void UpdateMatrix() {
		changedTransform = true;
	}

	void RecalculateTransform() {
		if (!changedTransform)
			return;
		std::lock_guard<std::mutex> matrixLock(matrixMutex);

		modelTransform = glm::mat4(1);

		modelTransform = glm::translate(modelTransform, position);

		modelTransform = glm::rotate(modelTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTransform = glm::scale(modelTransform, glm::vec3(scale, 1.0f));

		changedTransform = false;
	}

	bool changedTransform = false;

	glm::mat4 modelTransform = glm::mat4(1);
};

