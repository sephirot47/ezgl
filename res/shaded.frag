#version 430 core

uniform vec4 UColor;
uniform mat4 UView;
uniform vec3 UCameraWorldPosition;
uniform vec3 UCameraWorldDirection;

uniform vec4 ULightAmbientColor;
uniform vec4 ULightDiffuseColor;
uniform vec4 ULightSpecularColor;
uniform float ULightSpecularExponent;

layout(location = 0) in vec3 in_world_position;
layout(location = 1) in vec3 in_world_normal;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 cam_pos = UCameraWorldPosition;

    vec3 light_dir = normalize(vec3(-1.0, -1.0, 0.0));
    vec3 world_normal = normalize(in_world_normal);

    float diffuse_intensity = max(dot(-light_dir, world_normal), 0);

    vec3 light_reflected_dir = normalize(reflect(light_dir, world_normal));
    vec3 world_fragment_position_to_cam_dir = normalize(cam_pos - in_world_position);
    float specular_intensity = max(dot(light_reflected_dir, world_fragment_position_to_cam_dir), 0);
    specular_intensity = pow(specular_intensity, ULightSpecularExponent);

    vec4 lighted_color = vec4(0);
    lighted_color += ULightAmbientColor * UColor;
    lighted_color += diffuse_intensity * ULightDiffuseColor * UColor;
    lighted_color += specular_intensity * ULightSpecularColor;
    lighted_color.a = UColor.a;
    out_color = lighted_color;
}
