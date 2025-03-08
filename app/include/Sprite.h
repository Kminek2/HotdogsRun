#pragma once
#include "Image.h"
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <list>

#include "RectTransform.h"
#include "Model.h"
class Sprite
{
public:
	RectTransform* rectTransform;
	glm::vec3 color;

	Sprite(std::string name, glm::vec3 color = glm::vec3(1));
private:
	Sprite(glm::uvec2 texSize, glm::uint32_t offSet, std::string name);
	static void LoadImageFromFile(std::string name, std::string filePath);
	glm::uvec2 texSize;
	glm::uint32_t offSet;

	std::string name;

	std::list<Sprite*>::iterator i;

	static std::map<std::string, Sprite*> loadedSprites;
	static std::list<Sprite*> createdSprites;
	static Texture* textures;
	static const Model* imageModel;

	struct SpritaSendData {
		glm::vec2 texSize;
		glm::uint32_t offset;
		glm::vec3 color;
	};
};

