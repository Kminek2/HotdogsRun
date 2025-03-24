#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "CameraTransform.h"
#include "CameraStructures.h"
#include "UniformBuffer.h"

class Uniform;
class Camera
{
private:
	UniformCameraBuffer cameraBufferStruct;
	UniformBuffer<UniformCameraBuffer>* cameraBuffer;

	const float render_distance = 1000.0f;
	bool changedCubeMap = false;
	CubeMap* cubeMap;
public:
	CameraTransform* cameraTransform;
	ViewType view;

	static Camera* main;
	Camera(uint16_t frameNum, uint16_t width, uint16_t height);
	~Camera();
	void UpdateBuffer(uint16_t frame, Uniform* uniform);
	void UpdateCamera(uint16_t width, uint16_t height);

	void Reload(uint16_t width, uint16_t height);

	CubeMap* GetCubeMap() { return cubeMap; }
	void ChangeCubeMap(CubeMap* cubeMap);

	UniformBuffer<UniformCameraBuffer>* getBuffer() { return cameraBuffer; }

	float fov = 45.0f;
};

