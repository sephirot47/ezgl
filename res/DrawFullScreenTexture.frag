#version 430 core

uniform float UAlphaCutoff;
uniform sampler2D UTexture;
uniform sampler2D UDepthTexture;

layout(location = 0) in vec2 in_model_tex_coords;

layout(location = 0) out vec4 of_color;

void main()
{
  vec4 tex_color = texture(UTexture, in_model_tex_coords);

  float source_depth = texture(UDepthTexture, in_model_tex_coords).r;
  gl_FragDepth = source_depth;

  of_color = tex_color;
}
