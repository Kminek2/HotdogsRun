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

Sprite::Sprite(glm::uvec2 texSize, glm::uvec2 offSet, std::string name)
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
	loadedSprites[name] = new Sprite(textures->GetImageSize(filePath.c_str()), glm::uvec2(0, textures->dimention.second), name);
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


VkVertexInputBindingDescription Sprite::SpriteSendData::GetBindingDescription(uint16_t binding)
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = binding;
	bindingDescription.stride = sizeof(SpriteSendData);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
	return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> Sprite::SpriteSendData::GetAttributeDescriptions(uint16_t binding, uint16_t location)
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

	attributeDescriptions[0].binding = binding;
	attributeDescriptions[0].location = location;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_UINT;
	attributeDescriptions[0].offset = offsetof(SpriteSendData, texSize);

	attributeDescriptions[1].binding = binding;
	attributeDescriptions[1].location = location + 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32_UINT;
	attributeDescriptions[1].offset = offsetof(SpriteSendData, offset);

	attributeDescriptions[2].binding = binding;
	attributeDescriptions[2].location = location + 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(SpriteSendData, color);
	return attributeDescriptions;
}
