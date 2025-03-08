#include "Sprite.h"

std::map<std::string, Sprite*> Sprite::loadedSprites;
std::list<Sprite*> Sprite::createdSprites;
Texture* Sprite::textures;
const Model* Sprite::imageModel;// = Model::CreateUI("").first;

Sprite::Sprite(std::string name, glm::vec3 color)
{
	*this = *loadedSprites[name];
	createdSprites.push_back(this);
	i = std::prev(createdSprites.end());
}

Sprite::Sprite(glm::uvec2 texSize, glm::uint32_t offSet, std::string name)
{
	this->texSize = texSize;
	this->offSet = offSet;
	this->name = name;
}

void Sprite::LoadImageFromFile(std::string name, std::string filePath)
{
	loadedSprites[name] = new Sprite(textures->GetImageSize(filePath.c_str()), textures->dimention.second, name);
	textures->AddTexture(filePath.c_str());
}
