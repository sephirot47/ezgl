#version 430 core

uniform mat4 UModel;
uniform mat4 UView;
uniform mat4 UProjection;

layout(location = 0) in vec3 iv_model_position;
layout(location = 1) in vec3 iv_model_normal;

layout(location = 0) out vec3 ov_if_world_normal;

void main()
{
    ov_if_world_normal = normalize(UModel * vec4(iv_model_normal, 0)).xyz;
    gl_Position = UProjection * UView * UModel * vec4(iv_model_position, 1.0);
}
