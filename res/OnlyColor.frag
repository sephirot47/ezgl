#version 430 core

uniform vec4 UMaterialDiffuseColor;

layout(location = 0) out vec4 of_color;

void main()
{
    of_color = UMaterialDiffuseColor;
}
