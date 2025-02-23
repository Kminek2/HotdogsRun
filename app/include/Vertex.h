#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;

    static VkVertexInputBindingDescription GetBindingDescription(uint16_t binding = 0) {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = binding;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(uint16_t binding = 0, uint16_t location = 0) {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

        attributeDescriptions[0].binding = binding;
        attributeDescriptions[0].location = location;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = binding;
        attributeDescriptions[1].location = location + 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[2].binding = binding;
        attributeDescriptions[2].location = location + 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);
        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && texCoord == other.texCoord && normal == other.normal;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}