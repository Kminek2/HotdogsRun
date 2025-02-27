#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 model;
layout(location = 7) in uint textOff;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragPos;


void main() {
    fragPos = (model * vec4(inPosition, 1.0)).xyz;
    gl_Position = ubo.proj * ubo.view * vec4(fragPos, 1.0);
    fragTexCoord = texCoord;// + vec2(0, textOff);
    fragNormal = mat3(transpose(inverse(model))) * normal;
}