#version 430 core

uniform vec3 Color;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(Color, 1);
}
