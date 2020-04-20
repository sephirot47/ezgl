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
template <GL::EShaderType TShaderType, GL::EObjectType TShaderObjectType>
class Shader : public GLObject<TShaderObjectType>
{
  static_assert((TShaderType == GL::EShaderType::VERTEX && TShaderObjectType == GL::EObjectType::VERTEX_SHADER)
      || (TShaderType == GL::EShaderType::FRAGMENT && TShaderObjectType == GL::EObjectType::FRAGMENT_SHADER));

public:
  using GLObject<TShaderObjectType>::GetGLId;

  Shader() = default;
  explicit Shader(const std::filesystem::path& inSourceCodePath);
  explicit Shader(const std::string& inSourceCode);
  Shader(Shader&& inRHS) noexcept = default;
  virtual ~Shader() = default;

  void Compile(const std::string_view inSourceCode);
  static constexpr GL::EShaderType GetShaderType() { return TShaderType; }
};

using VertexShader = Shader<GL::EShaderType::VERTEX, GL::EObjectType::VERTEX_SHADER>;
using FragmentShader = Shader<GL::EShaderType::FRAGMENT, GL::EObjectType::FRAGMENT_SHADER>;
}

#include "Shader.tcc"