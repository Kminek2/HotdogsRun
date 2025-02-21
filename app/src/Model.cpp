#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <objLoader/tiny_obj_loader.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::map<std::string, Model*> Model::loadedModels;
std::list<Model*> Model::createdModels;
std::list<Model*> Model::createdUiModels;
Buffer<Vertex>* Model::vertexBuffer = new Buffer<Vertex>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
Buffer<uint32_t>* Model::indexBuffer = new Buffer<uint32_t>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
Texture* Model::textures = new Texture();
Buffer<uint32_t>* Model::textureOffBuffer = new Buffer<uint32_t>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
const glm::mat4 Model::startRot = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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

			vertex.texCoord = {
attrib.texcoords[2 * index.texcoord_index + 0],
1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};


			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);

		}
	}

	loadedModels[name] = new Model(std::move(vertices), std::move(indices));
	textures->AddTexture(texturePath.c_str());

}

Model* Model::Create(std::string model) {
	Model* newModel;

	try { newModel = new Model(*loadedModels.at(model), createdModels.end()); }
	catch (std::out_of_range) { throw std::runtime_error("Model not found"); }

	createdModels.push_back(newModel);
	newModel->iterator = std::next(createdModels.end(), -1);

	textureOffBuffer->ClearBuffer();
	for(Model* model : createdModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);
	for(Model* model : createdUiModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);

	textureOffBuffer->SendBufferToMemory();
	return newModel;
}

Model* Model::CreateUI(std::string model) {
	Model* newModel;

	try { newModel = new Model(*loadedModels.at(model), createdUiModels.end()); }
	catch (std::out_of_range) { throw std::runtime_error("Model not found"); }

	createdUiModels.push_back(newModel);
	newModel->iterator = std::next(createdUiModels.end(), -1);
	newModel->uiModel = true;

	textureOffBuffer->ClearBuffer();
	for (Model* model : createdModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);
	for (Model* model : createdUiModels)
		textureOffBuffer->AddToBuffer(model->textureOffset);

	textureOffBuffer->SendBufferToMemory();

	return newModel;
}

void Model::Delete() {
	if (uiModel)
		createdUiModels.erase(iterator);
	else
		createdModels.erase(iterator);
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
}

void Model::SendBuffers() {
	vertexBuffer->SendBufferToMemory();
	indexBuffer->SendBufferToMemory();

	textures->CreateSampler();
	textures->SendTexturesToMemory();
}

void Model::Unload() {
	delete vertexBuffer;
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