#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <objLoader/tiny_obj_loader.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::map<std::string, Model*> Model::loadedModels;
std::list<Model*> Model::createdModels;
std::list<Model*> Model::createdUiModels;
std::list<Model*> Model::createdSprites;
Buffer<Vertex>* Model::vertexBuffer = new Buffer<Vertex>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
Buffer<uint32_t>* Model::indexBuffer = new Buffer<uint32_t>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
Texture* Model::textures = new Texture();
Buffer<uint32_t>* Model::textureOffBuffer = new Buffer<uint32_t>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
const glm::mat4 Model::startRot = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
std::unordered_map<std::string, std::pair<std::list<Model*>::iterator, uint32_t>> Model::modelsIndtaces;
std::unordered_map<std::string, std::pair<std::list<Model*>::iterator, uint32_t>> Model::uiModelsIndtaces;

void Model::LoadEmptyModel() {
	loadedModels[""] = new Model({}, {});
	loadedModels[""]->modelName = "";
}

void Model::LoadModelFromFile(std::string name, std::string filePath, std::string texturePath, bool swichYZCoords)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	std::array<glm::vec2, 3> maxDist = {glm::vec2(0), glm::vec2(0) , glm::vec2(0) };

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.pos = {
attrib.vertices[3 * index.vertex_index + 0],
attrib.vertices[3 * index.vertex_index + 1],
attrib.vertices[3 * index.vertex_index + 2]
			};

			if(swichYZCoords)
				vertex.pos = startRot * glm::vec4(vertex.pos, 0);

			if (maxDist[0].x < vertex.pos.x)
				maxDist[0].x = vertex.pos.x;
			else if (maxDist[0].y > vertex.pos.x)
				maxDist[0].y = vertex.pos.x;

			if (maxDist[1].x < vertex.pos.y)
				maxDist[1].x = vertex.pos.y;
			else if (maxDist[1].y > vertex.pos.y)
				maxDist[1].y = vertex.pos.y;

			if (maxDist[2].x < vertex.pos.z)
				maxDist[2].x = vertex.pos.z;
			else if (maxDist[2].y > vertex.pos.z)
				maxDist[2].y = vertex.pos.z;

			vertex.texCoord = {
attrib.texcoords[2 * index.texcoord_index + 0],
1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				-attrib.normals[3 * index.normal_index + 2],
				attrib.normals[3 * index.normal_index + 1],
			};


			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);

		}
	}

	loadedModels[name] = new Model(std::move(vertices), std::move(indices));
	loadedModels[name]->modelName = name;
	loadedModels[name]->maxDistVert = maxDist;
	textures->AddTexture(texturePath.c_str());
}

std::pair<Model*, uint32_t> Model::Create(std::string model) {
	Model* newModel;

	try { newModel = new Model(*loadedModels.at(model), createdModels.end()); }
	catch (std::out_of_range) { throw std::runtime_error("Model not found"); }

	unsigned int index;
	if (!modelsIndtaces.contains(model)) {
		index = std::distance(createdModels.begin(), createdModels.end());
		createdModels.push_back(newModel);
		modelsIndtaces[model] = { std::prev(createdModels.end()), 1 };
	}
	else {
		createdModels.insert(modelsIndtaces[model].first, newModel);
		modelsIndtaces[model].second++;
		auto inter = modelsIndtaces[model].first;
		modelsIndtaces[model].first = std::prev(modelsIndtaces[model].first);
		index = std::distance(createdModels.begin(), modelsIndtaces[model].first);
	}
	newModel->iterator = modelsIndtaces[model].first;

	textureOffBuffer->ClearBuffer();
	for(Model* model : createdModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);
	for(Model* model : createdUiModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);

	textureOffBuffer->SendBufferToMemory();
	return { newModel, index };
}

std::pair<Model*, uint32_t> Model::CreateUI(std::string model) {
	Model* newModel;

	try { newModel = new Model(*loadedModels.at(model), createdUiModels.end()); }
	catch (std::out_of_range) { throw std::runtime_error("Model not found"); }

	unsigned int index;
	if (!uiModelsIndtaces.contains(model)) {
		index = std::distance(createdModels.begin(), createdModels.end());
		createdUiModels.push_back(newModel);
		uiModelsIndtaces[model] = { std::prev(createdUiModels.end()), 1 };
	}
	else {
		createdUiModels.insert(uiModelsIndtaces[model].first, newModel);
		uiModelsIndtaces[model].second++;
		uiModelsIndtaces[model].first = std::prev(uiModelsIndtaces[model].first);
		index = std::distance(createdUiModels.begin(), uiModelsIndtaces[model].first);
	}
	newModel->iterator = uiModelsIndtaces[model].first;
	newModel->uiModel = true;

	textureOffBuffer->ClearBuffer();
	for (Model* model : createdModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);
	for (Model* model : createdUiModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);

	textureOffBuffer->SendBufferToMemory();

	return { newModel, index };
}

Model* Model::CreateSprite()
{
	createdSprites++;
	return loadedModels["Sprite"];
}

void Model::DeleteSprite()
{
	createdSprites--;
}

void Model::Delete() {
	if (uiModel) {
		if (uiModelsIndtaces[modelName].first == iterator) {
			std::list<Model*>::iterator nextModel = createdUiModels.erase(iterator);
			if (uiModelsIndtaces[modelName].second == 1)
			{
				uiModelsIndtaces.erase(modelName);
			}
			else {
				uiModelsIndtaces[modelName] = { nextModel, uiModelsIndtaces[modelName].second - 1 };
			}
			return;
		}
		else {
			uiModelsIndtaces[modelName].second--;
		}

		std::list<Model*>::iterator nextModel = createdUiModels.erase(iterator);
	}
	else {
		if (modelsIndtaces[modelName].first == iterator) {
			std::list<Model*>::iterator nextModel = createdModels.erase(iterator);
			if (modelsIndtaces[modelName].second == 1)
			{
				modelsIndtaces.erase(modelName);
			}
			else {
				modelsIndtaces[modelName] = { nextModel, modelsIndtaces[modelName].second - 1 };
			}
			return;
		}
		else {
			modelsIndtaces[modelName].second--;
		}

		std::list<Model*>::iterator nextModel = createdModels.erase(iterator);
	}
}

Model::~Model() {
	Delete();
}

Model::Model(Model& model, std::list<Model*>::iterator iterator) {
	this->iterator = iterator;
	this->vertexOffset = model.vertexOffset;
	this->indexOffset = model.indexOffset;
	this->indexSize = model.indexSize;
	this->textureOffset = model.textureOffset;
	this->modelName = model.modelName;

	this->maxDistVert = model.maxDistVert;
}

std::string Model::GetName()
{
	return modelName;
}

std::array<glm::vec2, 3> Model::GetMaxDistVert()
{
	return maxDistVert;
}

void Model::SendBuffers() {
	vertexBuffer->SendBufferToMemory();
	indexBuffer->SendBufferToMemory();

	textures->CreateSampler();
	textures->SendTexturesToMemory();
}

void Model::Unload() {
	delete vertexBuffer;
	delete indexBuffer;
	delete textureOffBuffer;
	delete textures;
}

Model::Model(std::vector<Vertex> verticies, std::vector<uint32_t> indices) {
	this->vertexOffset = static_cast<uint32_t>(vertexBuffer->getSize());
	this->indexOffset = static_cast<uint32_t>(indexBuffer->getSize());
	this->indexSize = static_cast<uint32_t>(indices.size());
	this->textureOffset = static_cast<uint32_t>(textures->dimention.second);

	vertexBuffer->AddToBuffer(verticies);
	indexBuffer->AddToBuffer(indices);

	//MemoryCout();
}

void Model::MemoryCout(uint32_t verticies, uint32_t indicies) const {
	static const std::vector<char> after = { ' ', 'k', 'M', 'G' };

	double size = verticies * sizeof(Vertex);
	int iii = 0;
	while (size >= 1024)
	{
		size /= 1024;
		iii++;
	}

	std::cout << "memory taken = " << size << ' ' << after[iii] << "B\n\n\n";
}