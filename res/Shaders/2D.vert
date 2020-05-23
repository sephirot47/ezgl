R""(

#version 430 core

uniform mat3 UProjectionViewModel;

layout(location = 0) in vec3 in_model_position; // Z ignored
layout(location = 2) in vec2 in_texture_coordinates; // Z ignored

layout(location = 2) out vec2 out_texture_coordinates;

void main()
{
  vec2 pos = (UProjectionViewModel * vec3(in_model_position.xy, 1.0)).xy;
  out_texture_coordinates = in_texture_coordinates;
  gl_Position = vec4(pos, 0.0, 1.0);
}

)""