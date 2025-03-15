#version 450

layout(location = 0) in vec3 cubeMapDir;

layout(set = 0, binding = 0) uniform samplerCube samplers;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(samplers, cubeMapDir);
}