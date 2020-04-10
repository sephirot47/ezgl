#version 430 core

uniform vec3 UColor;

layout(location = 0) in vec3 iv_of_world_normal;

layout(location = 0) out vec4 of_color;

void main()
{
    vec3 light_dir = vec3(0, -1, 0);
    vec3 iv_of_world_normal_normalized = normalize(iv_of_world_normal);
    float light_intensity = max(dot(iv_of_world_normal_normalized, -light_dir), 0);
    light_intensity += 0.2f;

    vec3 lighted_color = light_intensity * UColor;
    of_color = vec4(lighted_color * 0.00001f + iv_of_world_normal_normalized, 1);
}
