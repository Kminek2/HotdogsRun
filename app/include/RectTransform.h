#pragma once
#include "Transform.h"
#include "Application.h"
class RectTransform :
    public Transform
{
public:
	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	/// <summary>
	/// Set position to the given vector, in world space
	/// </summary>
	void MoveTo(glm::vec2 position) {
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
		this->position += by;
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

	static void TransformToMemory(const std::vector<glm::mat4>& transforms) {
		transformBuffer->ClearBuffer();
		transformBuffer->AddToBuffer(transforms);
		transformBuffer->SendBufferToMemory();
	}

	RectTransform(glm::vec2 pos = glm::vec2(0), float rot = 0.0f, glm::vec2 scale = glm::vec2(1)) {
		position = pos;
		rotation = rot;
		this->scale = scale * glm::vec2(ASPECT_WIDTH, ASPECT_HEIGHT) / (float)std::max(ASPECT_WIDTH, ASPECT_HEIGHT);

		UpdateMatrix();
	}

	static void Unload() {
		delete transformBuffer;
	}

	glm::mat4 getModelMatrix() { return modelTransform; }
private:
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
		modelTransform = glm::mat4(1);

		modelTransform = glm::translate(modelTransform, glm::vec3(position, 0.0f));

		modelTransform = glm::rotate(modelTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTransform = glm::scale(modelTransform, glm::vec3(scale, 1.0f));
	}

	glm::mat4 modelTransform = glm::mat4(1);

	static Buffer<glm::mat4>* transformBuffer;
};

