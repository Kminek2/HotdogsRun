#version 450

struct SpriteInput{
    uvec2 texSize;
	vec3 color;
};

layout(set = 0, binding = 0) uniform Sprite {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 _a;

layout(location = 3) in uvec2 texSize;
layout(location = 4) in uvec2 offset;
layout(location = 5) in vec3 color;

layout(location = 6) in mat4 model;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out SpriteInput fragSpriteData;


void main() {
    fragTexCoord = texCoord + offset;

    gl_Position = model * vec4(inPosition, 1.0);
    gl_Position.y = - gl_Position.y;
    if(gl_Position.z < 0)
        gl_Position.z = 0;
    else if(gl_Position.z > 1)
        gl_Position.z = 1;

    fragSpriteData = SpriteInput(texSize, color);
}