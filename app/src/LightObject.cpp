#include "LightObject.h"
#include "Camera.h"

UniformBuffer<PointLightBuffer>* LightObject::pointLightBuffer;
UniformBuffer<SpotLightBuffer>* LightObject::spotLightBuffer;
DirLightBuffer LightObject::dirLight;
Shadow* LightObject::dirLightShadow = nullptr;

void LightObject::SetDirLight()
{
	dirLight.dir = glm::vec3(0);
}

LightObject::LightObject(GameObject* gameObject, glm::vec3 pos)
{
	this->gameObject = gameObject;
	this->pos = pos;
}

void LightObject::UpdateShadows()
{
	dirLightShadow->UpdateLightSpaceMatrix(glm::uvec2(100), Camera::main->cameraTransform->position, dirLight.dir);
}

void LightObject::SetDirLight(glm::vec3 dir, glm::vec3 ambientCol, glm::vec3 diffuseCol, glm::vec3 specularCol)
{
	dirLight.dir = glm::normalize(dir);
	dirLight.ambient = ambientCol;
	dirLight.diffuse = diffuseCol;
	dirLight.specular = specularCol;

	if(dirLightShadow == nullptr)
		dirLightShadow = new Shadow(glm::uvec2(100), dir);
}

void LightObject::SetDirLight(DirLightBuffer dirLight)
{
	dirLight.dir = glm::normalize(dirLight.dir);
	LightObject::dirLight = dirLight;

	if (dirLightShadow == nullptr)
		dirLightShadow = new Shadow(glm::uvec2(100), dirLight.dir);
}

void LightObject::Unload()
{
	delete pointLightBuffer;
	delete spotLightBuffer;
	if(dirLightShadow != nullptr)
		delete dirLightShadow;
}

void LightObject::DeleteAll()
{
	SetDirLight();

}
