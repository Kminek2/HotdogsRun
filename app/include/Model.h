#pragma once
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

#include <map>
#include <algorithm>
#include <iterator>
#include <string>
#include <list>

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Commands;
class DebugScene;

struct Model
{

	static void LoadModelFromFile(std::string name, std::string filePath, std::string texturePath);

	static Model* Create(std::string model);

	static Model* CreateUI(std::string model);

	void Delete();

	~Model();

	Model(Model& model, std::list<Model*>::iterator iterator);

	static void SendBuffers();

	static void Unload();

	static VkVertexInputBindingDescription GetBindingDescription(uint16_t binding = 2)
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = binding;
		bindingDescription.stride = sizeof(uint32_t);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		return bindingDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptions(uint16_t binding = 2, uint16_t location = 6) {
		VkVertexInputAttributeDescription attributeDescription{};

		attributeDescription.binding = binding;
		attributeDescription.location = location;
		attributeDescription.format = VK_FORMAT_R32_UINT;
		attributeDescription.offset = 0;

		return attributeDescription;
	}
private:
	Model(std::vector<Vertex> verticies, std::vector<uint32_t> indices);

	void MemoryCout(uint32_t verticies, uint32_t indicies) const;

	bool uiModel = false;

	static std::map<std::string, Model*> loadedModels;
	static std::list<Model*> createdModels;
	static std::list<Model*> createdUiModels;

	static Buffer<Vertex>* vertexBuffer;
	static Buffer<uint32_t>* indexBuffer;
	static Buffer<uint32_t>* textureOffBuffer;

	uint32_t vertexOffset;
	uint32_t indexOffset;
	uint32_t indexSize;
	uint32_t textureOffset;
	std::list<Model*>::iterator iterator;

	static Textures* textures;

	friend Commands;
	friend DebugScene;
	friend class Engine;
};