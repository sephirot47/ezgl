R""(

#version 430 core

uniform vec4 UMaterialColor;

layout(location = 2) in vec2 in_texture_coordinates;

layout(location = 0) out vec4 out_color;

void main()
{
  gl_FragDepth = 0.0f;
  out_color = UMaterialColor;
}

)""