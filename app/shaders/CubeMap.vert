#version 450

layout(set = 0, binding = 1) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec3 cubeMapDir;

void main() {
    cubeMapDir = inPosition;
    cubeMapDir.z = inPosition.y;
    cubeMapDir.y = inPosition.z;

    gl_Position = ubo.proj * mat4(mat3(ubo.view)) * vec4(inPosition, 1.0);
    gl_Position = gl_Position.xyww;
}