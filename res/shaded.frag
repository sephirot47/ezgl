#version 430 core

uniform vec4 UColor;
uniform mat4 UView;
uniform vec3 UCameraWorldDirection;

layout(location = 0) in vec3 in_world_position;
layout(location = 1) in vec3 in_world_normal;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 cam_pos = -UView[3].xyz;

    vec3 light_dir = normalize(vec3(-1.0, 0.0, 0.0));
    vec3 world_normal_normalized = normalize(in_world_normal);

    float diffuse_intensity = max(dot(world_normal_normalized, -light_dir), 0);

    vec3 light_reflected_dir = normalize(reflect(light_dir, world_normal_normalized));
    float specular_intensity = max(dot(light_reflected_dir, -UCameraWorldDirection), 0);
    specular_intensity = pow(specular_intensity, 120.0f);

    float ambient_intensity = 0.2f;

    float light_intensity = min(diffuse_intensity + specular_intensity + ambient_intensity, 1.0f);

    vec4 lighted_color = light_intensity * UColor;
    out_color = lighted_color;
}
