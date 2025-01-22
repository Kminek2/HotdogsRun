#include "Model.h"

std::map<std::string, Model*> Model::loadedModels;
std::list<Model*> Model::createdModels;
Buffer<Vertex>* Model::vertexBuffer = new Buffer<Vertex>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
Buffer<uint16_t>* Model::indexBuffer = new Buffer<uint16_t>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);