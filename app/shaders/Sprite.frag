#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) flat in vec4 fragColor;

layout(set = 0, binding = 0) uniform sampler2D samplers;

layout(location = 0) out vec4 outColor;

void main() {
    vec2 texCoord = fragTexCoord / textureSize(samplers, 0);
    outColor = texture(samplers, texCoord) * fragColor;
}