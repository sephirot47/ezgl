#version 430 core

uniform mat4 UModel;
uniform mat4 UNormal;
uniform mat4 UView;
uniform mat4 UProjection;
uniform mat4 UProjectionViewModel;

layout(location = 0) in vec3 in_model_position;
layout(location = 1) in vec3 in_model_normal;

layout(location = 0) out vec3 out_world_position;
layout(location = 1) out vec3 out_world_normal;

void main()
{
    out_world_position = (UModel * vec4(in_model_position, 1)).xyz;
    out_world_normal = (UNormal * vec4(in_model_normal, 0)).xyz;

    gl_Position = UProjectionViewModel * vec4(in_model_position, 1.0);
}
