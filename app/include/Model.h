#pragma once
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

#include <map>

#include <string>
#include <list>

class Commands;

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
};

struct Model
{

	static void LoadModelFromFile(std::string name, std::string filePath)
	{

		loadedModels[name] = new Model(vertices, indices);
	}

	static Model* Create(std::string model) {
		Model* newModel = new Model(*loadedModels[model], createdModels.end());;
		createdModels.push_back(newModel);
		newModel->iterator = std::next(createdModels.end(), -1);
		return newModel;
	}

	void Delete() {
		createdModels.erase(iterator);
	}

	Model() {

	}

	~Model() {
		Delete();
	}

	Model(Model& model, std::list<Model*>::iterator iterator) {
		this->iterator = iterator;
		this->indexOffset = model.indexOffset;
		this->indexSize = model.indexSize;
	}

	static void SendBuffers() {
		vertexBuffer->SendBufferToMemory();
		indexBuffer->SendBufferToMemory();
	}

	static void Unload() {
		delete vertexBuffer;
		delete indexBuffer;
	}
private:
	Model(std::vector<Vertex> verticies, std::vector<uint16_t>indicies) {
		uint32_t indexBufferSize = static_cast<uint32_t>(indexBuffer->getSize());
		this->indexOffset = indexBufferSize;
		this->indexSize = static_cast<uint32_t>(indicies.size());

		for (int i = 0; i < indicies.size(); i++)
		{
			indicies[i] = indicies[i] + indexBufferSize;
		}

		vertexBuffer->AddToBuffer(verticies);
		indexBuffer->AddToBuffer(indicies);
	}

	static std::map<std::string, Model*> loadedModels;
	static std::list<Model*> createdModels;

	static Buffer<Vertex>* vertexBuffer;
	static Buffer<uint16_t>* indexBuffer;

	uint32_t indexOffset;
	std::list<Model*>::iterator iterator;

	uint32_t indexSize;

	friend Commands;
};