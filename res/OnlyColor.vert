#version 430 core

uniform mat4 UModel;
uniform mat4 UNormal;
uniform mat4 UView;
uniform mat4 UProjection;
uniform mat4 UProjectionViewModel;

layout(location = 0) in vec3 iv_model_position;

void main()
{
  gl_Position = UProjectionViewModel * vec4(iv_model_position, 1.0);
}
