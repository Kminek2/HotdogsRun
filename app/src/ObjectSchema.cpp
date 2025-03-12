#include "ObjectSchema.hpp"

int ObjectSchema::GetSurfaceType(const std::string& type) {
	return sur_ids.at(type);
};

std::map<std::string, int> ObjectSchema::sur_ids;