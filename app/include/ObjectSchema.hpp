#pragma once

#include "glm/vec3.hpp"
#include <string>
#include <map>

struct ObjectSchema {
	std::string model = "";
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	// -1: def; 0: none; 1: road; 2: dirt; 3: ice
	int surface_type = -1;

	ObjectSchema(std::string model, glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1), int surface_type = -1) : model(model), scale(scale), rotation(rotation), surface_type(surface_type) {
		sur_ids[""] = -1;
		sur_ids["Asfalt"] = 1;
		sur_ids["Zwir"] = 2;
		sur_ids["Lod"] = 3;
	};

	static int GetSurfaceType(const std::string& type);

private:
	static std::map<std::string, int> sur_ids;
};