#pragma once
#include "Image.h"
#include "UniformBuffer.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shadow
{
private:
	Image lightDepthImage;
	VkDeviceMemory depthImageMemory;
	VkSampler depthImageSampler;

	glm::mat4 lightSpaceMatrix;
public:
	Shadow(glm::uvec2 size, glm::vec3 dir);
	~Shadow();


};

