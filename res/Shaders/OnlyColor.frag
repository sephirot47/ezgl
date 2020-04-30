R""(

#version 430 core

uniform vec4 UMaterialDiffuseColor;

layout(location = 0) out vec4 out_color;

void main()
{
    out_color = UMaterialDiffuseColor;
}

)""