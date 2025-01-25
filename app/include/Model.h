#pragma once
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

#include <map>

#include <string>
#include <list>

class Commands;
class DebugScene;

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
};

struct Model
{

	static void LoadModelFromFile(std::string name, std::string filePath)
	{

		loadedModels[name] = new Model(vertices);
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
		this->vertexOffset = model.vertexOffset;
		this->vertexSize = model.vertexSize;
	}

	static void SendBuffers() {
		vertexBuffer->SendBufferToMemory();
	}

	static void Unload() {
		delete vertexBuffer;
	}
private:
	Model(std::vector<Vertex> verticies) {
		this->vertexOffset = static_cast<uint32_t>(vertexBuffer->getSize());
		this->vertexSize = static_cast<uint32_t>(verticies.size());

		vertexBuffer->AddToBuffer(verticies);
	}

	static std::map<std::string, Model*> loadedModels;
	static std::list<Model*> createdModels;

	static Buffer<Vertex>* vertexBuffer;

	uint32_t vertexOffset;
	std::list<Model*>::iterator iterator;

	uint32_t vertexSize;

	friend Commands;
	friend DebugScene;
};