R""(

#version 430 core

uniform mat4 UProjectionViewModel;

layout(location = 0) in vec3 in_model_position;
layout(location = 2) in vec2 in_model_texture_coordinate;

layout(location = 2) out vec2 out_texture_coordinate;

void main()
{
  out_texture_coordinate = in_model_texture_coordinate;
  gl_Position = UProjectionViewModel * vec4(in_model_position, 1.0);
}

)""