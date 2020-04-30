R""(

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
layout(std140, binding = 0) uniform UBlockDirectionalLights
{
  DirectionalLight UDirectionalLights[MAX_NUMBER_OF_DIRECTIONAL_LIGHTS];
};

struct PointLight
{
  vec3 mPosition;
  float mRange;

  vec3 mColor;
  float PADDING_1;
};
const int MAX_NUMBER_OF_POINT_LIGHTS = 100;
uniform int UNumberOfPointLights;
layout(std140, binding = 0) uniform UBlockPointLights { PointLight UPointLights[MAX_NUMBER_OF_POINT_LIGHTS]; };

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

vec3 ComputeLight(in vec3 inLightDir,
    in vec3 inLightColor,
    in vec3 inWorldPosition,
    in vec3 inWorldNormal,
    in vec3 inCamPosition,
    in vec3 inMaterialDiffuseColor)
{
  float diffuse_intensity = max(dot(-inLightDir, inWorldNormal), 0);

  vec3 light_reflected_dir = normalize(reflect(inLightDir, inWorldNormal));
  vec3 world_fragment_position_to_cam_dir = normalize(inCamPosition - inWorldPosition);
  float specular_intensity = max(dot(light_reflected_dir, world_fragment_position_to_cam_dir), 0);
  specular_intensity = min(pow(specular_intensity, UMaterialSpecularExponent), 1.0);
  specular_intensity *= UMaterialSpecularIntensity;

  vec3 diffuse_light_apportation = diffuse_intensity * inLightColor * inMaterialDiffuseColor;
  vec3 specular_diffuse_apportation = specular_intensity * inLightColor;
  vec3 light_apportation = (diffuse_light_apportation + specular_diffuse_apportation);
  return light_apportation;
}

void main()
{
  vec4 material_color = UMaterialDiffuseColor * texture(UMaterialTexture, in_texture_coordinate);
  if (UMaterialLightingEnabled)
  {
    vec3 cam_pos = UCameraWorldPosition;
    vec3 world_normal = normalize(in_world_normal);

    vec3 color_lighted = (USceneAmbientColor * material_color.rgb);

    // Directional lights
    for (int i = 0; i < UNumberOfDirectionalLights; ++i)
    {
      vec3 light_dir = UDirectionalLights[i].mDirection;
      vec3 light_color = UDirectionalLights[i].mColor;

      vec3 light_apportation
          = ComputeLight(light_dir, light_color, in_world_position, world_normal, cam_pos, material_color.rgb);
      color_lighted += light_apportation;
    }

    // Point lights
    for (int i = 0; i < UNumberOfPointLights; ++i)
    {
      vec3 light_position = UPointLights[i].mPosition;
      float light_range = UPointLights[i].mRange;
      vec3 light_color = UPointLights[i].mColor;
      vec3 light_dir = normalize(in_world_position - light_position);

      float light_distance = distance(light_position, in_world_position);
      float light_falloff = max((light_range - light_distance), 0.0f) / light_range;

      vec3 light_apportation
          = ComputeLight(light_dir, light_color, in_world_position, world_normal, cam_pos, material_color.rgb);
      light_apportation *= light_falloff;
      color_lighted += light_apportation;
    }

    out_color = vec4(color_lighted, material_color.a);
  }
  else
  {
    out_color = material_color;
  }
}

)""