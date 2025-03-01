#include "objects\Lights.h"
#include "GameObject.h"

const std::map<std::string, std::vector<PointLightBuffer>> Lights::pointLights;
const std::map<std::string, std::vector<SpotLightBuffer>> Lights::spotLights = {
	//		NAME                    POS					COL					FUNC				DIR				CUTOFF
	{"bilboardHodDog", {{{ 2.05f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}},
						{{ 1.35f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}},
						{{ 0.65f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}},
						{{ -0.05f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}},
						{{ -0.75f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}},
						{{ -1.45f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}},
						{{ -2.15f, -1.55f, 5.6f }, {0.4, 0.4, 0.1}, {1.0f, 0.09f, 0.032f}, {0.0f, 1.0f, 1.0f}, {12.5f, 15}}}
	}
};

void Lights::Init()
{
	std::string name = gameObject->GetModelName();

	if (pointLights.contains(name))
		for (const auto& light : pointLights.at(name))
			new PointLight(gameObject, light);

	if(spotLights.contains(name))
		for (const auto& light : spotLights.at(name))
			new SpotLight(gameObject, light);
}

void Lights::Update()
{
}

void Lights::OnDestroy()
{
}
