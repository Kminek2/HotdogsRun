#version 450

struct SpriteInput{
    vec2 texSize;
	vec3 color;
};

layout(set = 0, binding = 0) uniform Sprite {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 1) in vec2 texSize;
layout(location = 2) in uint offset;
layout(location = 3) in vec3 color;

layout(location = 4) in mat4 model;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out SpriteInput fragSpriteData;


void main() {
    fragTexCoord = inPosition.xy + vec2(0, offset);

    gl_Position = model * vec4(inPosition, 1.0);
    gl_Position.y = - gl_Position.y;
    if(gl_Position.z < 0)
        gl_Position.z = 0;
    else if(gl_Position.z > 1)
        gl_Position.z = 1;

    fragSpriteData = SpriteInput(texSize, color);
}