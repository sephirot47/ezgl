#pragma once

#include "FileUtils.h"
#include "GL.h"
#include "GLObject.h"
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace egl
{
template <GL::EShaderType TShaderType>
class Shader : public GLObject<GL::GetObjectType<TShaderType>()>
{
public:
  static constexpr auto GLObjectType = GL::GetObjectType<TShaderType>();
  using GLObject<GLObjectType>::GetGLId;

  Shader() = default;
  explicit Shader(const std::filesystem::path& inSourceCodePath);
  explicit Shader(const std::string& inSourceCode);
  Shader(Shader&& inRHS) noexcept = default;
  virtual ~Shader() = default;

  void Compile(const std::string_view inSourceCode);
  static constexpr GL::EShaderType GetShaderType() { return TShaderType; }
};

using VertexShader = Shader<GL::EShaderType::VERTEX>;
using FragmentShader = Shader<GL::EShaderType::FRAGMENT>;
}

#include "Shader.tcc"