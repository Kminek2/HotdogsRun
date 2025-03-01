#pragma once
#include "ObjectScript.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <map>
#include <vector>
class Lights :
    public ObjectScript
{
private:
    static const std::map<std::string, std::vector<PointLightBuffer>> pointLights;
    static const std::map<std::string, std::vector<SpotLightBuffer>> spotLights;
public:
    void Init() override;
    void Update() override;
    void OnDestroy() override;
};

