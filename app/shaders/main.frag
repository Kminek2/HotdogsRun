#version 450

layout(constant_id = 0) const int MAX_TEXTURES = 64;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in flat uint textureNum;

layout(set = 0, binding = 1) uniform sampler2D samplers[MAX_TEXTURES];

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(samplers[textureNum], fragTexCoord);
}