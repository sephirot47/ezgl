R""(

#version 430 core

uniform sampler2D UMaterialTexture;
uniform vec4 UMaterialDiffuseColor;

layout(location = 2) in vec2 in_texture_coordinate;

layout(location = 0) out vec4 out_color;

void main()
{
  float atlas_texture_value = texture2D(UMaterialTexture, vec2(in_texture_coordinate.x, in_texture_coordinate.y)).r;
  // out_color = vec4(in_texture_coordinate, 0, 1) + atlas_texture_value * 0.00001f;
  if (atlas_texture_value == 0.0f)
    discard;

  out_color = UMaterialDiffuseColor * vec4(1, 1, 1, atlas_texture_value);
}

)""