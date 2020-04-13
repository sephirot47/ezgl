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

template <typename T>
void ShaderProgram::SetUniformSafe(const std::string_view inName, const T& inValue)
{
  const auto uniform_location = GetUniformLocation(inName);
  if (uniform_location.has_value())
    SetUniform<T>(*uniform_location, inValue);
}
}