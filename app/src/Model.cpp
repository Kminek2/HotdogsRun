#include "Model.h"

std::map<std::string, Model*> Model::loadedModels;
std::list<Model*> Model::createdModels;
Buffer<Vertex>* Model::vertexBuffer = new Buffer<Vertex>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
Buffer<uint16_t>* Model::indexBuffer = new Buffer<uint16_t>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

std::vector<Vertex> Model::convert_model_data(const vox::model_data model_data) {
	std::vector<Vertex> vertices;
	vertices.reserve(model_data.vertices.size());

	std::transform(model_data.vertices.begin(), model_data.vertices.end(), model_data.colors.begin(), std::back_inserter(vertices),
		[](const auto& vert, const auto& col) -> Vertex {
			return {
				glm::vec3{
					std::get<0>(vert),
					std::get<1>(vert),
					std::get<2>(vert)
				},
				glm::vec3{
					static_cast<float>(std::get<0>(col.second)),
					static_cast<float>(std::get<1>(col.second)),
					static_cast<float>(std::get<2>(col.second))
				}
			};
		});

	return vertices;
}