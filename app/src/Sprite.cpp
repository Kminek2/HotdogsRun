#include "Sprite.h"

std::map<std::string, Sprite*> Sprite::loadedSprites;
std::list<Sprite*> Sprite::createdSprites;
Texture* Sprite::textures = new Texture();
Buffer<Sprite::SpriteSendData>* Sprite::spriteBuffer = new Buffer<Sprite::SpriteSendData>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
const std::string Sprite::spriteModelName = "Sprite";
std::mutex Sprite::createdSpriteMutex;

Sprite::Sprite(std::string name, glm::vec4 color)
{
	model = Model::CreateSprite();

	*this = *loadedSprites[name];
	this->color = color;
	std::lock_guard<std::mutex> createdSpritesLock(createdSpriteMutex);
	createdSprites.push_back(this);
	i = std::prev(createdSprites.end());
	rectTransform = new RectTransform();
}

Sprite::Sprite(std::string name, glm::uvec2 texSize, glm::uvec2 offset, glm::vec4 color)
{
	model = Model::CreateSprite();

	*this = *loadedSprites[name];
	this->texSize = texSize;
	this->offSet += offset;
	this->color = color;
	std::lock_guard<std::mutex> createdSpritesLock(createdSpriteMutex);
	createdSprites.push_back(this);
	i = std::prev(createdSprites.end());
	rectTransform = new RectTransform();
}

Sprite::Sprite(std::string name, int, glm::vec2 texSize01, glm::vec2 offset01, glm::vec4 color)
{
	model = Model::CreateSprite();

	*this = *loadedSprites[name];
	this->offSet += offset01 * (glm::vec2)this->texSize;
	this->texSize = (glm::vec2)texSize * texSize01;
	this->color = color;
	std::lock_guard<std::mutex> createdSpritesLock(createdSpriteMutex);
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
	color = glm::vec4(1);

	model = nullptr;
}

void Sprite::LoadImageFromFile(std::string name, std::string filePath)
{
	loadedSprites[name] = new Sprite(textures->GetImageSize(filePath.c_str()), glm::uvec2(0, textures->dimention.second), name);
	textures->AddTexture(filePath.c_str());
}

Sprite::~Sprite()
{
	std::unique_lock<std::mutex> createdSpritesLock(createdSpriteMutex, std::defer_lock);
	createdSpritesLock.lock();
	if (!createdSprites.empty())
		createdSprites.erase(i);

	createdSpritesLock.unlock();
	model->DeleteSprite();

	if(rectTransform != nullptr)
		delete rectTransform;
}

void Sprite::DeleteAll()
{
	std::unique_lock<std::mutex> createdSpritesLock(createdSpriteMutex, std::defer_lock);
	createdSpritesLock.lock();
	std::list<Sprite*>::iterator i = createdSprites.begin();
	while (i != createdSprites.end()) {
		std::list<Sprite*>::iterator i2 = std::next(i);
		createdSpritesLock.unlock();
		delete (*i);
		createdSpritesLock.lock();
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
	delete spriteBuffer;
}

void Sprite::SendDataToGPU()
{
	textures->CreateSampler();
	textures->SendTexturesToMemory();
}

void Sprite::UpdateBuffer()
{
	std::lock_guard<std::mutex> createdSpritesLock(createdSpriteMutex);
	std::list<Sprite*>::iterator i = createdSprites.begin();

	std::vector<glm::mat4> matrixes;
	std::vector<Sprite::SpriteSendData> sprites;

	while (i != createdSprites.end()) {
		matrixes.push_back((*i)->rectTransform->getModelMatrix());
		SpriteSendData sendData{};
		sendData.color = (*i)->color;
		sendData.texSize = (*i)->texSize;
		sendData.offset = (*i)->offSet;

		sprites.push_back(sendData);

		i = std::next(i);
	}

	Transform::AddToMemory(matrixes);
	
	spriteBuffer->ClearBuffer();
	spriteBuffer->AddToBuffer(sprites);
	spriteBuffer->SendBufferToMemory();
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
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(SpriteSendData, color);
	return attributeDescriptions;
}
