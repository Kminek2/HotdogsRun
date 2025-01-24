#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "Transform.h"
#include "CameraStructures.h"
#include "UniformBuffer.h"
class Camera
{
private:
	UniformCameraBuffer cameraBufferStruct;
	UniformBuffer<UniformCameraBuffer>* cameraBuffer;

	float yaw = 0;
	float pitch = 0;

	glm::vec3 direction;
	glm::vec3 front;
	glm::vec3 up;
public:

	glm::vec3 pos;

	static Camera* main;
	Camera(uint16_t frameNum, uint16_t width, uint16_t height);
	~Camera();
	void UpdateBuffer(uint16_t frame);
	void UpdateCamera(uint16_t width, uint16_t height);

	void RotateCamera(float x, float y);
	void MoveCameraTo(glm::vec3 pos);
	void MoveCamera(glm::vec3 by);
	void MoveCamera(float by);

	void Reload(uint16_t width, uint16_t height);

	ViewType view;

	UniformBuffer<UniformCameraBuffer>* getBuffer() { return cameraBuffer; }
};

