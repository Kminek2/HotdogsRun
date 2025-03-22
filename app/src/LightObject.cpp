#include "LightObject.h"
#include "Camera.h"

UniformBuffer<PointLightBuffer>* LightObject::pointLightBuffer;
UniformBuffer<SpotLightBuffer>* LightObject::spotLightBuffer;
DirLightBuffer LightObject::dirLight;
glm::uvec2 LightObject::shadowSize = glm::uvec2(1);
Shadow* LightObject::dirLightShadow = nullptr;

void LightObject::SetDirLight()
{
	dirLight.dir = glm::vec3(0);
	shadowSize = glm::uvec2(0);
}

void LightObject::Init()
{
	dirLightShadow = new Shadow(glm::uvec2(100), glm::vec3(0, 0, -1));
}

LightObject::LightObject(GameObject* gameObject, glm::vec3 pos)
{
	this->gameObject = gameObject;
	this->pos = pos;
}

void LightObject::UpdateShadows()
{
	if(dirLightShadow != nullptr)
		dirLightShadow->UpdateLightSpaceMatrix(shadowSize, Camera::main->cameraTransform->position, dirLight.dir);
}

void LightObject::SetDirLight(glm::vec3 dir, glm::vec3 ambientCol, glm::vec3 diffuseCol, glm::vec3 specularCol)
{
	dirLight.dir = glm::normalize(dir);
	dirLight.ambient = ambientCol;
	dirLight.diffuse = diffuseCol;
	dirLight.specular = specularCol;

	shadowSize = glm::uvec2(100);

	if(dirLightShadow == nullptr)
		dirLightShadow = new Shadow(shadowSize, dir);

	UpdateShadows();
}

void LightObject::SetDirLight(DirLightBuffer dirLight)
{
	dirLight.dir = glm::normalize(dirLight.dir);
	LightObject::dirLight = dirLight;

	shadowSize = glm::uvec2(100);

	if (dirLightShadow == nullptr)
		dirLightShadow = new Shadow(shadowSize, dirLight.dir);

	UpdateShadows();
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
