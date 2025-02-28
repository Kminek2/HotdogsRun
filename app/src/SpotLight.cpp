#include "SpotLight.h"
#include "GameObject.h"

std::list<SpotLight*> SpotLight::createdLightObjects;
uint32_t SpotLight::lightNum = 0;

uint32_t SpotLight::SendData(uint16_t currentFrame)
{
	std::list<SpotLight*>::iterator i = createdLightObjects.begin();
	std::vector<SpotLightBuffer> spotLights;
	while (i != createdLightObjects.end()) {
		SpotLightBuffer spotLight = (*i)->light;
		spotLight.pos = glm::vec3((*i)->gameObject->transform->getModelMatrix() * glm::vec4((*i)->light.pos, 1));
		spotLights.push_back(spotLight);
		i = std::next(i);
	}

	spotLightBuffer->UpdateBuffer(currentFrame, *spotLights.data(), spotLights.size() * sizeof(SpotLightBuffer));
	return spotLights.size() * sizeof(SpotLightBuffer);
}

void SpotLight::DeleteAll()
{
	while (lightNum > 0)
		delete *createdLightObjects.begin();
}

SpotLight::SpotLight(GameObject* gameObjec, glm::vec3 pos, glm::vec3 dir, glm::vec3 col, glm::vec2 cutOffs, float constant, float linear, float quadratic) : LightObject(gameObjec, pos)
{
	lightNum++;
	light.pos = pos;
	light.col = col;
	light.direction = glm::normalize(dir);
	light.functions = glm::vec3(constant, linear, quadratic);
	light.cutOffs = cutOffs;

	createdLightObjects.push_back(this);
	i = std::prev(createdLightObjects.end());
}

SpotLight::~SpotLight()
{
	lightNum--;
	createdLightObjects.erase(i);
}
