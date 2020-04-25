#version 430 core

uniform mat4 UProjectionViewModel;

layout(location = 0) in vec3 model_position;

void main() { gl_Position = UProjectionViewModel * vec4(model_position, 1.0); }
