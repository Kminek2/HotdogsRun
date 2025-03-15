#include "Camera.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SwapChain.h"

#include <iostream>

#include <algorithm>

#include "Uniform.h"

Camera* Camera::main = nullptr;

Camera::Camera(uint16_t frameNum, uint16_t width, uint16_t height)
{
	main = this;
	cameraTransform = new CameraTransform(glm::vec3(0.0f, 2.0f, 2.0f));

	cameraBuffer = new UniformBuffer<UniformCameraBuffer>(frameNum);
	view = Perspective;
	if (view == Perspective)
		cameraBufferStruct.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, Camera::render_distance);
	else
		cameraBufferStruct.proj = glm::ortho(-16.0f, 16.0f, -8.0f, 8.0f, 0.1f, Camera::render_distance);

	cameraBufferStruct.proj[1][1] *= -1;
	cameraBufferStruct.view = glm::lookAt(cameraTransform->position, cameraTransform->position + cameraTransform->front, cameraTransform->up);

	cubeMap = nullptr;
}

Camera::~Camera() {
	delete cameraTransform;
	delete cameraBuffer;
}

void Camera::UpdateCamera(uint16_t width, uint16_t height)
{
	if (view == Perspective)
		cameraBufferStruct.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, Camera::render_distance);
	else
		cameraBufferStruct.proj = glm::ortho(-16.0f, 16.0f, -8.0f, 8.0f, 0.1f, Camera::render_distance);
	cameraBufferStruct.proj[1][1] *= -1;
	cameraBufferStruct.view = glm::lookAt(cameraTransform->position, cameraTransform->position + cameraTransform->front, cameraTransform->up);
}

void Camera::UpdateBuffer(uint16_t frame, Uniform* uniform)
{
	cameraBuffer->UpdateBuffer(frame, cameraBufferStruct);

	if (changedCubeMap) {
		uniform->UpdateImageInDescriptorSets(cubeMap->sampler, cubeMap->imageView, 0);
	}
}

void Camera::Reload(uint16_t width, uint16_t height)
{
	cameraTransform->MoveTo(glm::vec3(0.0f, 2.0f, 2.0f));
	cameraTransform->front = glm::vec3(0, -1, 0);
	cameraTransform->up = glm::vec3(0, 0, 1);

	view = Perspective;
	if (view == Perspective)
		cameraBufferStruct.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, Camera::render_distance);
	else
		cameraBufferStruct.proj = glm::ortho(-16, 16, -8, 8);

	cameraBufferStruct.proj[1][1] *= -1;
	cameraBufferStruct.view = glm::lookAt(cameraTransform->position, cameraTransform->position + cameraTransform->front, cameraTransform->up);
}

void Camera::ChangeCubeMap(CubeMap* cubeMap)
{
	if (this->cubeMap != nullptr)
		delete this->cubeMap;

	this->cubeMap = cubeMap;

	changedCubeMap = true;
}
