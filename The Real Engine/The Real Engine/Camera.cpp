#include "Camera.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SwapChain.h"

#include <iostream>

#include <algorithm>

Camera* Camera::main;

Camera::Camera(uint16_t frameNum, uint16_t width, uint16_t height)
{
	main = this;
	MoveCameraTo(glm::vec3(0.0f, 2.0f, 2.0f));
	front = glm::vec3(0, -1, 0);
	up = glm::vec3(0, 0, 1);

	cameraBuffer = new UniformBuffer<UniformCameraBuffer>(frameNum);
	view = Perspective;
	if (view == Perspective)
		cameraBufferStruct.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
	else
		cameraBufferStruct.proj = glm::ortho(-16, 16, -8, 8);

	cameraBufferStruct.proj[1][1] *= -1;
	cameraBufferStruct.view = glm::lookAt(pos, pos + front, up);
}

Camera::~Camera() {
	delete cameraBuffer;
}

void Camera::UpdateCamera(uint16_t width, uint16_t height)
{
	if (view == Perspective)
		cameraBufferStruct.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
	else
		cameraBufferStruct.proj = glm::ortho(-16, 16, -8, 8);
	cameraBufferStruct.proj[1][1] *= -1;
	cameraBufferStruct.view = glm::lookAt(pos, pos + front, up);
}

void Camera::RotateCamera(float x, float y)
{
	pitch -= y;
	yaw -= x;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.z = sin(glm::radians(pitch));
	direction.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
	glm::vec3 upDir;

	upDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch + 90));
	upDir.z = sin(glm::radians(pitch + 90));
	upDir.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch + 90));

	up = glm::normalize(upDir);
}

void Camera::UpdateBuffer(uint16_t frame)
{
	/*for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << cameraBufferStruct.proj[i][j] << ' ';
		}
		std::cout << '\n';
	}

	std::cout << "\n\n\n";*/
	cameraBuffer->UpdateBuffer(frame, cameraBufferStruct);
}

void Camera::MoveCameraTo(glm::vec3 pos) {
	this->pos = pos;
}

void Camera::MoveCamera(glm::vec3 by)
{
	this->pos += front * by;
}

void Camera::Reload(uint16_t width, uint16_t height)
{
	MoveCameraTo(glm::vec3(0.0f, 2.0f, 2.0f));
	front = glm::vec3(0, -1, 0);
	up = glm::vec3(0, 0, 1);

	view = Perspective;
	if (view == Perspective)
		cameraBufferStruct.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
	else
		cameraBufferStruct.proj = glm::ortho(-16, 16, -8, 8);

	cameraBufferStruct.proj[1][1] *= -1;
	cameraBufferStruct.view = glm::lookAt(pos, pos + front, up);
}

void Camera::MoveCamera(float by)
{
	this->pos += front * by;
}
