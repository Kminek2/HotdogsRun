#pragma once
#include "GameObject.h"

class UiObject : public GameObject
{
public:
	UiObject(std::string model, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	~UiObject();
};

