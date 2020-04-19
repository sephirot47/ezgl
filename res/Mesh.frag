#version 430 core

struct DirectionalLight
{
  vec3 mDirection;
  float PADDING_0;

  vec3 mColor;
  float PADDING_1;
};

const int MAX_NUMBER_OF_DIRECTIONAL_LIGHTS = 100;
uniform int UNumberOfDirectionalLights;
layout(std140, binding = 0) uniform UBlockLights
{
  DirectionalLight UDirectionalLights[MAX_NUMBER_OF_DIRECTIONAL_LIGHTS];
};

uniform mat4 UView;
uniform vec3 UCameraWorldPosition;
uniform vec3 UCameraWorldDirection;

uniform sampler2D UMaterialTexture;
uniform vec4 UMaterialDiffuseColor;
uniform float UMaterialSpecularIntensity;
uniform float UMaterialSpecularExponent;
uniform bool UMaterialLightingEnabled;

uniform vec3 USceneAmbientColor;

layout(location = 0) in vec3 in_world_position;
layout(location = 1) in vec3 in_world_normal;
layout(location = 2) in vec2 in_texture_coordinate;

layout(location = 0) out vec4 out_color;

void main()
{
  vec4 material_color = UMaterialDiffuseColor * texture(UMaterialTexture, in_texture_coordinate);
  if (UMaterialLightingEnabled)
  {
    vec3 cam_pos = UCameraWorldPosition;
    vec3 world_fragment_position_to_cam_dir = normalize(cam_pos - in_world_position);
    vec3 world_normal = normalize(in_world_normal);

    vec3 color_lighted = (USceneAmbientColor * material_color.rgb);
    for (int i = 0; i < UNumberOfDirectionalLights; ++i)
    {
      vec3 light_dir = UDirectionalLights[i].mDirection;
      vec3 light_color = UDirectionalLights[i].mColor;

      float diffuse_intensity = max(dot(-light_dir, world_normal), 0);

      vec3 light_reflected_dir = normalize(reflect(light_dir, world_normal));
      float specular_intensity = max(dot(light_reflected_dir, world_fragment_position_to_cam_dir), 0);
      specular_intensity = pow(specular_intensity, UMaterialSpecularExponent);
      specular_intensity *= UMaterialSpecularIntensity;

      color_lighted += diffuse_intensity * light_color * material_color.rgb;
      color_lighted += specular_intensity * light_color;
    }
    out_color = vec4(color_lighted, material_color.a);
  }
  else
  {
    out_color = material_color;
  }
}
