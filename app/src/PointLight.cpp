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
		pointLight.pos = (*i)->gameObject->transform->modelTransform * glm::vec4((*i)->light.pos, 0);
		pointLights.push_back(pointLight);
		i = std::next(i);
	}

	pointLightBuffer->UpdateBuffer(currentFrame, pointLights.data(), pointLights.size() * sizeof(PointLightBuffer));
	return pointLights.size() * sizeof(PointLightBuffer);
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
