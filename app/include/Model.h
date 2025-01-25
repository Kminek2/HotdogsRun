#pragma once
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

#include <map>

#include <string>
#include <list>

#include "vox/Vox.hpp"

class Commands;
class DebugScene;

struct Model
{

	static void LoadModelFromFile(std::string name, std::string filePath)
	{
		vox::model_data tmp;
		if (vox::load_model("models/"+filePath, tmp) != 0)
			throw std::runtime_error("Failed to load model from file");

		loadedModels[name] = new Model(tmp);
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

	Model() {}

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

	Model(const vox::model_data model_data) : Model(convert_model_data(model_data)) {};
	std::vector<Vertex> convert_model_data(const vox::model_data model_data);

	static std::map<std::string, Model*> loadedModels;
	static std::list<Model*> createdModels;

	static Buffer<Vertex>* vertexBuffer;

	uint32_t vertexOffset;
	std::list<Model*>::iterator iterator;

	uint32_t vertexSize;

	friend Commands;
	friend DebugScene;
};