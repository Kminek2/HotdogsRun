#pragma once
#include "LightObject.h"
#include <list>

class SpotLight :
    public LightObject
{
private:
    SpotLightBuffer light;
    static uint32_t SendData(uint16_t currentFrame);
    static std::list<SpotLight*> createdLightObjects;
    static uint32_t lightNum;
    std::list<SpotLight*>::iterator i;
    friend class Engine;
    friend class Commands;
    friend Application;
public:
    SpotLight(GameObject* gameObjec, glm::vec3 pos, glm::vec3 dir, glm::vec3 col = glm::vec3(1), glm::vec2 cutOffs = glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))), float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
};

