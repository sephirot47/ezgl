#include "ezgl/Shader.h"

namespace egl
{
template <GL::EShaderType TShaderType>
Shader<TShaderType>::Shader(const std::string_view inSourceCode) : Shader()
{
  Compile(inSourceCode);
}

template <GL::EShaderType TShaderType>
void Shader<TShaderType>::Compile(const std::string_view inSourceCode)
{
  GL::ShaderSource(GetGLId(), inSourceCode);
  GL::CompileShader(GetGLId());

  const auto compiled_correctly = GL::GetShaderInteger(GetGLId(), GL::EShaderInfo::COMPILE_STATUS);
  if (!compiled_correctly)
  {
    const auto error_msg = GL::GetShaderInfoLog(GetGLId());
    THROW_EXCEPTION("Error compiling shader: " << error_msg);
  }
}
}