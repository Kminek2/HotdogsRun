#version 450

struct SpriteInput{
    uvec2 texSize;
	vec3 color;
};

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) flat in SpriteInput fragSpriteData;

layout(set = 0, binding = 0) uniform sampler2D samplers;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(samplers, vec2(fragTexCoord.x * (fragSpriteData.texSize.x / textureSize(samplers, 0).x), fragTexCoord.y * (fragSpriteData.texSize.y / textureSize(samplers, 0).y))) * vec4(fragSpriteData.color, 1);
}