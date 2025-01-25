#pragma once
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

#include <map>

#include <string>
#include <list>

#include "vox/Vox.hpp"

class Commands;

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

	Model(const vox::model_data model_data) : Model(convert_model_data(model_data), model_data.indices) {};
	std::vector<Vertex> convert_model_data(const vox::model_data model_data);

	static std::map<std::string, Model*> loadedModels;
	static std::list<Model*> createdModels;

	static Buffer<Vertex>* vertexBuffer;
	static Buffer<uint16_t>* indexBuffer;

	uint32_t indexOffset;
	std::list<Model*>::iterator iterator;

	uint32_t indexSize;

	friend Commands;
};