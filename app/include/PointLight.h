#pragma once
#include "LightObject.h"
#include <list>
class PointLight :
    public LightObject
{
private:
    PointLightBuffer light;
    static uint32_t SendData(uint16_t currentFrame);
    static std::list<PointLight*> createdLightObjects;
    static uint32_t lightNum;
    std::list<PointLight*>::iterator i;
    static void DeleteAll();
    friend class Engine;
    friend class Commands;
    friend Application;
public:
    PointLight(GameObject* gameObjec, glm::vec3 pos, glm::vec3 col = glm::vec3(1), float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
    ~PointLight();
};

