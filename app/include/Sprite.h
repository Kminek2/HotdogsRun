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
	glm::vec4 color;

	Sprite(std::string name, glm::vec4 color = glm::vec4(1));
	Sprite(std::string name, glm::uvec2 texSize, glm::uvec2 offset, glm::vec4 color = glm::vec4(1));
	Sprite(std::string name, int, glm::vec2 texSize01, glm::vec2 offset01, glm::vec4 color = glm::vec4(1));
	~Sprite();
private:
	Sprite(glm::uvec2 texSize, glm::uvec2 offSet, std::string name);
	static void DeleteAll();
	static void Unload();
	static void LoadImageFromFile(std::string name, std::string filePath);
	static void SendDataToGPU();
	static void UpdateBuffer();
	glm::uvec2 texSize;
	glm::uvec2 offSet;

	Model* model;
	std::string name;

	std::list<Sprite*>::iterator i;

	static std::map<std::string, Sprite*> loadedSprites;
	static std::mutex createdSpriteMutex;
	static std::list<Sprite*> createdSprites;
	static Texture* textures;

	struct SpriteSendData {
		glm::uvec2 texSize;
		glm::uvec2 offset;
		glm::vec4 color;

		static VkVertexInputBindingDescription GetBindingDescription(uint16_t binding = 0);

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(uint16_t binding = 0, uint16_t location = 1);
	};

	static Buffer<SpriteSendData>* spriteBuffer;

	const static std::string spriteModelName;

	friend class Application;
	friend class RenderPass;
	friend class Commands;
};

