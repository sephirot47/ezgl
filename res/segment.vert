#version 430 core

uniform mat4 UModel;
uniform mat4 UView;
uniform mat4 UProjection;

layout(location = 0) in vec3 iv_model_position;

void main()
{
  gl_Position = UProjection * UView * UModel * vec4(iv_model_position, 1.0);
}
