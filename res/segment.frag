#version 430 core

uniform vec4 UColor;

layout(location = 0) out vec4 of_color;

void main()
{
    of_color = UColor;
}
