#include "LightObject.h"

UniformBuffer<PointLightBuffer>* LightObject::pointLightBuffer;
UniformBuffer<SpotLightBuffer>* LightObject::spotLightBuffer;
DirLightBuffer LightObject::dirLight;

void LightObject::SetDirLight()
{
	dirLight.dir = glm::vec3(0);
}

LightObject::LightObject(GameObject* gameObject, glm::vec3 pos)
{
	this->gameObject = gameObject;
	this->pos = pos;
}

void LightObject::SetDirLight(glm::vec3 dir, glm::vec3 ambientCol, glm::vec3 diffuseCol, glm::vec3 specularCol)
{
	dirLight.dir = glm::normalize(dir);
	dirLight.ambient = ambientCol;
	dirLight.diffuse = diffuseCol;
	dirLight.specular = specularCol;
}

void LightObject::SetDirLight(DirLightBuffer dirLight)
{
	dirLight.dir = glm::normalize(dirLight.dir);
	LightObject::dirLight = dirLight;
}

void LightObject::Unload()
{
	delete pointLightBuffer;
	delete spotLightBuffer;
}

void LightObject::DeleteAll()
{
	SetDirLight();

}
