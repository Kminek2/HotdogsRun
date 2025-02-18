#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in mat4 model;
layout(location = 6) in uint textOff;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out uint textureNum;


void main() {
    gl_Position = model * vec4(inPosition, 1.0);
    if(gl_Position.z < 0)
        gl_Position.z = 0;
    else if(gl_Position.z > 1)
        gl_Position.z = 1;

    textureNum = textOff;
}