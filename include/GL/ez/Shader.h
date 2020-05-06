#pragma once

#include "ez/FileUtils.h"
#include "ez/GL.h"
#include "ez/GLObject.h"
#include <cstdint>
#include <string>
#include <string_view>

namespace ez
{
template <GL::EShaderType TShaderType>
class Shader final : public GLObject<GL::GetObjectType<TShaderType>()>
{
public:
  static constexpr auto GLObjectType = GL::GetObjectType<TShaderType>();
  using GLObject<GLObjectType>::GetGLId;

  Shader() = default;
  explicit Shader(const std::string_view inSourceCode);
  Shader(Shader&& inRHS) noexcept = default;
  virtual ~Shader() = default;

  void Compile(const std::string_view inSourceCode);
  static constexpr GL::EShaderType GetShaderType() { return TShaderType; }
};

using VertexShader = Shader<GL::EShaderType::VERTEX>;
using FragmentShader = Shader<GL::EShaderType::FRAGMENT>;
}

#include "ez/Shader.tcc"