#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

struct InstColorChange{
    uint index;
    uint amount;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 model;
layout(location = 7) in uint textOff;
layout(location = 8) in uint ColorChangeAmount;
layout(location = 9) in uint ColorChangeIndex;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragPos;
layout(location = 3) flat out InstColorChange colorChange;


void main() {
    fragPos = (model * vec4(inPosition, 1.0)).xyz;
    gl_Position = ubo.proj * ubo.view * vec4(fragPos, 1.0);
    fragTexCoord = texCoord;// + vec2(0, textOff);
    fragNormal = mat3(transpose(inverse(model))) * normal;
    colorChange.index = ColorChangeIndex;
    colorChange.amount = ColorChangeAmount;
}