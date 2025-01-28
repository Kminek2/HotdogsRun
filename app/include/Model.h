#pragma once
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

#include <map>
#include <algorithm>
#include <iterator>
#include <string>
#include <list>

class Commands;
class DebugScene;

struct Model
{

	static void LoadModelFromFile(std::string name, std::string filePath)
	{
		std::vector<vox::vertex> tmp;
		if (vox::load_model("models/"+filePath, tmp) != 0)
			throw std::runtime_error("Failed to load model from file");

		std::vector<Vertex> vertices;
		vertices.reserve(tmp.size());
		std::transform(tmp.begin(), tmp.end(), std::back_inserter(vertices), [](const vox::vertex& v) { return Vertex(v); });

		loadedModels[name] = new Model(std::move(vertices));
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