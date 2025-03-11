#include "Sprite.h"

std::map<std::string, Sprite*> Sprite::loadedSprites;
std::list<Sprite*> Sprite::createdSprites;
Texture* Sprite::textures = new Texture();

Sprite::Sprite(std::string name, glm::vec3 color)
{
	model = Model::CreateSprite();

	*this = *loadedSprites[name];
	createdSprites.push_back(this);
	i = std::prev(createdSprites.end());
	rectTransform = new RectTransform();
}

Sprite::Sprite(glm::uvec2 texSize, glm::uint32_t offSet, std::string name)
{
	this->texSize = texSize;
	this->offSet = offSet;
	this->name = name;
	rectTransform = nullptr;
	color = glm::vec3(1);

	model = nullptr;
}

void Sprite::LoadImageFromFile(std::string name, std::string filePath)
{
	loadedSprites[name] = new Sprite(textures->GetImageSize(filePath.c_str()), textures->dimention.second, name);
	textures->AddTexture(filePath.c_str());
}

Sprite::~Sprite()
{
	if (!createdSprites.empty())
		createdSprites.erase(i);
	model->DeleteSprite();

	if(rectTransform != nullptr)
		delete rectTransform;
}

void Sprite::DeleteAll()
{
	std::list<Sprite*>::iterator i = createdSprites.begin();
	while (i != createdSprites.end()) {
		std::list<Sprite*>::iterator i2 = std::next(i);
		delete (*i);
		i = i2;
	}
}

void Sprite::Unload()
{
	DeleteAll();
	for (auto& sp : loadedSprites)
		delete sp.second;
	loadedSprites.clear();

	delete textures;
}

void Sprite::SendDataToGPU()
{
	textures->CreateSampler();
	textures->SendTexturesToMemory();
}