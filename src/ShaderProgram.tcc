#include "GL.h"
#include "ShaderProgram.h"

namespace egl
{
template <typename T>
void ShaderProgram::SetUniform(const GL::Id inUniformLocation, const T& inValue)
{
  EXPECTS(IsBound());
  EXPECTS(inUniformLocation != GL::InvalidId);
  GL::Uniform(inUniformLocation, inValue);
}

template <typename T>
void ShaderProgram::SetUniform(const std::string_view inName, const T& inValue)
{
  SetUniform<T>(GetUniformLocationWithException(*this, inName), inValue);
}
}