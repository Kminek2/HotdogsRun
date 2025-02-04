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

class Commands;
class DebugScene;

struct Model
{

	static void LoadModelFromFile(std::string name, std::string filePath)
	{
		std::vector<vox::vertex> tmp;
		if (vox::load_model(filePath, tmp) != 0)
			throw std::runtime_error("Failed to load model from file");

		std::vector<Vertex> vertices;
		vertices.reserve(tmp.size());
		std::transform(tmp.begin(), tmp.end(), std::back_inserter(vertices), [](const vox::vertex& v) { return Vertex(v); });

		loadedModels[name] = new Model(std::move(vertices));
	}

	static Model* Create(std::string model) {
		Model* newModel;

		try { newModel = new Model(*loadedModels.at(model), createdModels.end()); } 
		catch (std::out_of_range) { throw std::runtime_error("Model not found"); }

		createdModels.push_back(newModel);
		newModel->iterator = std::next(createdModels.end(), -1);
		return newModel;
	}

	static Model* CreateUI(std::string model) {
		Model* newModel;

		try { newModel = new Model(*loadedModels.at(model), createdUiModels.end()); }
		catch (std::out_of_range) { throw std::runtime_error("Model not found"); }

		createdUiModels.push_back(newModel);
		newModel->iterator = std::next(createdUiModels.end(), -1);
		newModel->uiModel = true;
		return newModel;
	}

	void Delete() {
		if (uiModel)
			createdUiModels.erase(iterator);
		else
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

		MemoryCout();
	}

	void MemoryCout() const {
		static const std::vector<char> after = { ' ', 'k', 'M', 'G' };

		double size = vertexSize * sizeof(Vertex);
		int iii = 0;
		while (size >= 1024)
		{
			size /= 1024;
			iii++;
		}

		std::cout << "memory taken = " << size << ' ' << after[iii] << "B\n\n\n";
	}

	bool uiModel = false;

	static std::map<std::string, Model*> loadedModels;
	static std::list<Model*> createdModels;
	static std::list<Model*> createdUiModels;

	static Buffer<Vertex>* vertexBuffer;

	uint32_t vertexOffset;
	std::list<Model*>::iterator iterator;

	uint32_t vertexSize;

	friend Commands;
	friend DebugScene;
};