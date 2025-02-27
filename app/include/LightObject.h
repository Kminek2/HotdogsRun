#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "UniformBuffer.h"
#include "LightBufferStruct.h"

class Application;
class GameObject;

class LightObject
{
public:
	GameObject* gameObject;
	glm::vec3 pos;
	static void SetDirLight(glm::vec3 dir, glm::vec3 ambientCol, glm::vec3 diffuseCol, glm::vec3 specularCol );
	static void SetDirLight(DirLightBuffer dirLight);
protected:
	LightObject(GameObject* gameObject, glm::vec3 pos);
	static UniformBuffer<PointLightBuffer*>* pointLightBuffer;
	static UniformBuffer<SpotLightBuffer*>* spotLightBuffer;
private:

	static DirLightBuffer dirLight;

	static UniformBuffer<PointLightBuffer*>* getPointBuffer() { return pointLightBuffer; }
	static UniformBuffer<SpotLightBuffer*>* getSpotBuffer() { return spotLightBuffer; }
	static void Unload();
	friend Engine;
	friend Application;
	friend Commands;
};

