#version 430 core

layout(location = 0) in vec3 in_model_position;
layout(location = 2) in vec2 in_model_tex_coords;

layout(location = 0) out vec2 out_model_tex_coords;

void main()
{
  out_model_tex_coords = in_model_tex_coords;
  gl_Position = vec4(in_model_position.xy, 0.0, 1.0);
}
