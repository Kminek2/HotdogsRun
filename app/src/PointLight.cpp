#include "PointLight.h"
#include <vector>
#include "GameObject.h"

std::list<PointLight*> PointLight::createdLightObjects;
uint32_t PointLight::lightNum = 0;

uint32_t PointLight::SendData(uint16_t currentFrame)
{
	std::list<PointLight*>::iterator i = createdLightObjects.begin();
	std::vector<PointLightBuffer> pointLights;
	while (i != createdLightObjects.end()) {
		PointLightBuffer pointLight = (*i)->light;
		pointLight.pos = glm::vec3((*i)->gameObject->transform->getModelMatrix() * glm::vec4((*i)->light.pos, 1));
		pointLights.push_back(pointLight);
		i = std::next(i);
	}

	pointLightBuffer->UpdateBuffer(currentFrame, *pointLights.data(), pointLights.size() * sizeof(PointLightBuffer));
	return pointLights.size() * sizeof(PointLightBuffer);
}

void PointLight::DeleteAll()
{
	while (lightNum > 0)
		delete* createdLightObjects.begin();
}

PointLight::PointLight(GameObject* gameObjec, glm::vec3 pos, glm::vec3 col, float constant, float linear, float quadratic) : LightObject(gameObjec, pos)
{
	lightNum++;
	light.pos = pos;
	light.col = col;
	light.functions = glm::vec3(constant, linear, quadratic);

	createdLightObjects.push_back(this);
	i = std::prev(createdLightObjects.end());
}

PointLight::~PointLight()
{
	lightNum--;
	createdLightObjects.erase(i);
}
