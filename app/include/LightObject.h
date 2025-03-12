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
	/// <summary>
	/// Sets dir light with provided values
	/// </summary>
	/// <param name="dir">direction. It's automaticly normalized</param>
	/// <param name="ambientCol">color if light doesn't hit fragment</param>
	/// <param name="diffuseCol">color if light hits the fragment</param>
	/// <param name="specularCol">color of light reflections</param>
	static void SetDirLight(glm::vec3 dir, glm::vec3 ambientCol, glm::vec3 diffuseCol, glm::vec3 specularCol );
	/// <summary>
	/// Sets dir light to provided one.
	/// Direction is normalized.
	/// </summary>
	/// <param name="dirLight"></param>
	static void SetDirLight(DirLightBuffer dirLight);
	/// <summary>
	/// Sets dir light to NULL
	/// </summary>
	static void SetDirLight();
protected:
	LightObject(GameObject* gameObject, glm::vec3 pos);
	static UniformBuffer<PointLightBuffer>* pointLightBuffer;
	static UniformBuffer<SpotLightBuffer>* spotLightBuffer;
private:

	static DirLightBuffer dirLight;

	static UniformBuffer<PointLightBuffer>* getPointBuffer() { return pointLightBuffer; }
	static UniformBuffer<SpotLightBuffer>* getSpotBuffer() { return spotLightBuffer; }
	static void Unload();
	static void DeleteAll();
	friend Engine;
	friend Application;
	friend Commands;
	friend class RenderPass;
};

