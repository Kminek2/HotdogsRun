#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <list>
#include <algorithm>

#include "Buffer.h"

class GameObject;

namespace glm {
	constexpr uint8_t elements(const glm::vec3& v) { return std::count_if(&v.x, &v.x + 3, [](float f) { return f != 0; }); }
};

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 front = { 0, 0, 1 };
	glm::vec3 right = { 1, 0, 0 };
	glm::vec3 up    = { 0, 1, 0 };

	static VkVertexInputBindingDescription GetBindingDescription(uint16_t binding = 0)
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = binding;
		bindingDescription.stride = sizeof(glm::mat4);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(uint16_t binding = 0, uint16_t location = 2) {
		std::vector<VkVertexInputAttributeDescription> attributeDescription(4);

		attributeDescription[0].binding = binding;
		attributeDescription[0].location = location;
		attributeDescription[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescription[0].offset = 0;

		attributeDescription[1].binding = binding;
		attributeDescription[1].location = location + 1;
		attributeDescription[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescription[1].offset = sizeof(glm::vec4);

		attributeDescription[2].binding = binding;
		attributeDescription[2].location = location + 2;
		attributeDescription[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescription[2].offset = 2 * sizeof(glm::vec4);

		attributeDescription[3].binding = binding;
		attributeDescription[3].location = location + 3;
		attributeDescription[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescription[3].offset = 3 * sizeof(glm::vec4);

		return attributeDescription;
	}

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
	void RotateTo(glm::vec3 rotation) {
		this->rotation = ModuloRotation(rotation);
		UpdateVectors();
		UpdateMatrix();
	}

	/// <summary>
	/// Set scale to the given vector
	/// </summary>
	void ScaleTo(glm::vec3 scale) {
		this->scale = scale;
		UpdateMatrix();
	}

	/// <summary>
	/// Move the object by the given vector, in world space
	/// </summary>
	void Move(glm::vec3 by) {
		this->position += by;
		UpdateMatrix();
	}

	/// <summary>
	/// Move the object by the given amount in the direction of the front vector
	/// </summary>
	void Translate(float by) {
		this->position += this->front * by;
		UpdateMatrix();
	}

	void Translate(glm::vec3 by);

	/// <summary>
	/// Rotates the object by the given amount on each axis, in degrees
	/// </summary>
	void Rotate(glm::vec3 by) {
		this->rotation = ModuloRotation(this->rotation + by);

		UpdateVectors();
		UpdateMatrix();
	}

	/// <summary>
	/// Increases the scale of the object by the given amount on each axis
	/// </summary>
	void Scale(glm::vec3 by) {
		this->scale += by;
		UpdateMatrix();
	}

	/// <summary>
	/// Increases the scale of the object by the given amount on all axes
	/// </summary>
	void Scale(float by) {
		this->scale += glm::vec3(by);
		UpdateMatrix();
	}

	void UpdateVectors() {
		glm::mat4 rot(1.0f);
		rot = glm::rotate(rot, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		rot = glm::rotate(rot, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rot = glm::rotate(rot, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::vec3 baseForward = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 baseUp = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::vec3 newFront = glm::vec3(rot * glm::vec4(baseForward, 0.0f));
		glm::vec3 newUp = glm::vec3(rot * glm::vec4(baseUp, 0.0f));

		newFront = glm::normalize(newFront);
		newUp = glm::normalize(newUp);

		glm::vec3 newRight = glm::normalize(glm::cross(newFront, newUp));

		this->front = newFront;
		this->up = newUp;
		this->right = newRight;
	}

	static void TransformToMemory(const std::vector<glm::mat4>& transforms) {
		transformBuffer->ClearBuffer();
		transformBuffer->AddToBuffer(transforms);
		transformBuffer->SendBufferToMemory();
	}

	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1)) {
		position = pos;
		rotation = rot;
		this->scale = scale;

		UpdateVectors();
		UpdateMatrix();
	}

private:
	/// <summary>
	/// Takes in a rotation vector in degrees, and clamps each angle to [0, 360)
	/// </summary>
	/// <returns>(glm::vec3) -> (glm::vec3)</returns>
	glm::vec3 ModuloRotation(glm::vec3 rotation) {
		rotation = glm::vec3(ClampRotation(rotation.x), ClampRotation(rotation.y), ClampRotation(rotation.z));
		return rotation;
	}

	/// <summary>
	/// Takes in an angle in degrees, and returns the angle clamped to [0, 360)
	/// </summary>
	/// <returns>(float) -> (float)</returns>
	float ClampRotation(float r) {
		while (r >= 360.0f) r -= 360.0f;
		while (r <    0.0f) r += 360.0f;
		return r;
	}

	void UpdateMatrix() {
		modelTransform = glm::mat4(1);

		modelTransform = glm::translate(modelTransform, position);

		modelTransform = glm::rotate(modelTransform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTransform = glm::rotate(modelTransform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTransform = glm::rotate(modelTransform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

		modelTransform = glm::scale(modelTransform, scale);
	}

	glm::mat4 modelTransform = glm::mat4(1);

	static Buffer<glm::mat4>* transformBuffer;

	friend class Commands;
	friend class GameObject;
};