#pragma once

#include "glm/vec3.hpp"
#include <string>

struct ObjectSchema {
	std::string model = "";
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	ObjectSchema(std::string model, glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1)) : model(model), scale(scale), rotation(rotation) {};
};