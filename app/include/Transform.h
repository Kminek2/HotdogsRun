#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <list>

#include "Buffer.h"

class GameObject;

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 front = { 0, 0, 1 };
	glm::vec3 right = { 1, 0, 0};
	glm::vec3 up = {0, 1, 0};

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

	void MoveTo(glm::vec3 position) {
		this->position = position;
		UpdateMatrix();
	}

	void RotateTo(glm::vec3 rotation) {
		this->rotation = ModuloRotation(rotation);
		UpdateVectors();
		UpdateMatrix();
	}

	void ScaleTo(glm::vec3 scale) {
		this->scale = scale;
		UpdateMatrix();
	}

	void Move(glm::vec3 by) {
		this->position += by;
		UpdateMatrix();
	}

	void Translate(float by) {
		this->position += this->front * by;
		UpdateMatrix();
	}

	void Rotate(glm::vec3 by) {
		this->rotation += by;
		rotation = ModuloRotation(rotation);
		UpdateMatrix();
	}

	void Scale(glm::vec3 by) {
		this->scale += by;
		UpdateMatrix();
	}

	void Scale(float by) {
		this->scale += glm::vec3(by);
		UpdateMatrix();
	}

	void UpdateVectors() {
		glm::vec3 front;
		float x, y, z;
		front.x = -cos(glm::radians(rotation.y - 90.0f)) * cos(glm::radians(this->rotation.x));
		front.y = sin(glm::radians(rotation.x));
		front.z = sin(glm::radians(rotation.y - 90.0f)) * cos(glm::radians(this->rotation.x));
		this->front = glm::normalize(front);

		right.x = cos(glm::radians(rotation.z));
		right.y = sin(glm::radians(rotation.z));
		right.z = 0.0f;
		right = glm::normalize(right);

		this->up = -glm::normalize(glm::cross(this->front, this->right));
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

	glm::vec3 ModuloRotation(glm::vec3 rotation) {
		rotation = glm::vec3(ClampRotation(rotation.x), ClampRotation(rotation.y), ClampRotation(rotation.z));
		return rotation;
	}

	float ClampRotation(float r) {
		if (r > 360)
			r -= 360;
		else if (r < 360)
			r += 360;

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