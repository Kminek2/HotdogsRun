#include "Model.h"

std::map<std::string, Model*> Model::loadedModels;
std::list<Model*> Model::createdModels;
Buffer<Vertex>* Model::vertexBuffer = new Buffer<Vertex>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);