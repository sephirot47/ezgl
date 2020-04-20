#pragma once

#include "GL.h"
#include "GLBindableObject.h"
#include "Mat.h"
#include "Shader.h"
#include "Vec.h"
#include <cstdint>
#include <optional>
#include <string_view>

namespace egl
{
class ShaderProgram : public GLBindableObject<GL::EBindingType::SHADER_PROGRAM>
{
public:
  using GLBindableObject<GL::EBindingType::SHADER_PROGRAM>::GetGLId;

  ShaderProgram(const VertexShader& inVertexShader, const FragmentShader& inFragmentShader);
  ShaderProgram(ShaderProgram&& ioRHS) = default;
  ~ShaderProgram() = default;

  std::optional<GL::Id> GetAttribLocation(const std::string_view inAttribName) const;
  std::optional<GL::Id> GetUniformLocation(const std::string_view inUniformName) const;
  std::optional<GL::Id> GetUniformBlockIndex(const std::string_view inUniformBlockName) const;

  template <typename T>
  void SetUniform(const GL::Id inUniformLocation, const T& inValue);
  template <typename T>
  void SetUniform(const std::string_view inUniformName, const T& inValue);
  template <typename T>
  void SetUniformSafe(const std::string_view inUniformName, const T& inValue);
  void SetUniformBlockBinding(const std::string_view inUniformBlockName, const GL::Id inBindingPoint);
  void SetUniformBlockBinding(const GL::Id inUniformBlockIndex, const GL::Id inBindingPoint);
  void SetUniformBlockBindingSafe(const std::string_view inUniformBlockName, const GL::Id inBindingPoint);

private:
  GL::Id GetUniformLocationWithException(const ShaderProgram& inShaderProgram, const std::string_view inUniformName);
};
}

#include "ShaderProgram.tcc"
