#version 430 core

uniform mat4 Model;

layout(location = 0) in vec3 position;

void main()
{
    gl_Position = Model * vec4(position, 1.0);
}
