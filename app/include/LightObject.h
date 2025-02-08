#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "UniformBuffer.h"
#include "LightBufferStruct.h"

class Application;

class LightObject
{
private:
	static UniformBuffer<LightBufferStruct>* lightBuffer;

	static UniformBuffer<LightBufferStruct>* getBuffer() { return lightBuffer; }
	friend Engine;
	friend Application;
};

