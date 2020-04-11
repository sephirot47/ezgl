#include "Shader.h"
#include "GL.h"
#include "Macros.h"
#include <iostream>

namespace egl
{
Shader::Shader(const GL::EShaderType inShaderType, const std::string_view inSourceCode)
    : mGLId(GL::CreateShader(inShaderType))
{
  if (mGLId == 0)
    THROW_EXCEPTION("Error creating shader of type " << GL::EnumCast(inShaderType));

  GL::ShaderSource(mGLId, inSourceCode);
  GL::CompileShader(mGLId);

  const auto compiled_correctly = GL::GetShaderInteger(mGLId, GL::EShaderInfo::COMPILE_STATUS);
  if (!compiled_correctly)
  {
    const auto error_msg = GL::GetShaderInfoLog(mGLId);
    THROW_EXCEPTION("Error compiling shader: " << error_msg);
  }
}

Shader::~Shader() { GL::DeleteShader(mGLId); }
}
