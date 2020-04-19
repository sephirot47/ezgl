#version 430 core

struct DirectionalLight
{
  vec3 Direction;
  vec3 Color;
};

layout (std140, binding = 0) uniform UDirectionalLights {
  DirectionalLight lights[100];
};

uniform mat4 UView;
uniform vec3 UCameraWorldPosition;
uniform vec3 UCameraWorldDirection;

uniform sampler2D UMaterialTexture;
uniform vec4 UMaterialDiffuseColor;
uniform float UMaterialSpecularIntensity;
uniform float UMaterialSpecularExponent;
uniform bool UMaterialLightingEnabled;

uniform vec4 USceneAmbientColor;
uniform vec4 ULightColor;

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

    vec3 world_normal = normalize(in_world_normal);
    vec3 light_dir = normalize(vec3(0.0, 0.0, -1.0));

    float diffuse_intensity = max(dot(-light_dir, world_normal), 0);

    vec3 light_reflected_dir = normalize(reflect(light_dir, world_normal));
    vec3 world_fragment_position_to_cam_dir = normalize(cam_pos - in_world_position);
    float specular_intensity = max(dot(light_reflected_dir, world_fragment_position_to_cam_dir), 0);
    specular_intensity = pow(specular_intensity, UMaterialSpecularExponent);
    specular_intensity *= UMaterialSpecularIntensity;

    vec4 lighted_color = vec4(0);
    lighted_color += USceneAmbientColor * material_color;
    lighted_color += diffuse_intensity * ULightColor * material_color;
    lighted_color += specular_intensity * ULightColor;
    lighted_color.a = material_color.a;
    // out_color = vec4(in_world_normal, 1);
    out_color = lighted_color;
  }
  else
  {
    out_color = material_color;
  }
}