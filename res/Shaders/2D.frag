R""(

#version 430 core

uniform vec4 UMaterialColor;

layout(location = 0) out vec4 of_color;

void main()
{
  gl_FragDepth = 0.0f;
  of_color = UMaterialColor;
}

)""