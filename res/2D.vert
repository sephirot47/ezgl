#version 430 core

uniform mat3 UProjectionViewModel;

layout(location = 0) in vec3 model_position; // Z ignored

void main()
{
  vec2 pos = (UProjectionViewModel * vec3(model_position.xy, 1.0)).xy;
  gl_Position = vec4(pos, 0.0, 1.0);
}
