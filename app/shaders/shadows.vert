#version 450

 layout(set = 0, binding = 0) uniform lightPos{
    mat4 lightSpaceMat;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in mat4 model;

void main(){
    gl_Position = lightSpaceMat * model * vec4(inPosition, 1.0);
}