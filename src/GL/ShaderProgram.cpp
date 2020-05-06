#include "ez/ShaderProgram.h"
#include "ez/GL.h"
#include "ez/Macros.h"
#include "ez/Shader.h"
#include <GL/glew.h>
#include <cassert>
#include <stdexcept>

namespace ez
{
ShaderProgram::ShaderProgram(const VertexShader& inVertexShader, const FragmentShader& inFragmentShader)
{
  GL::AttachShader(GetGLId(), inVertexShader.GetGLId());
  GL::AttachShader(GetGLId(), inFragmentShader.GetGLId());
  GL::LinkProgram(GetGLId());
}

std::optional<GL::Id> ShaderProgram::GetAttribLocation(const std::string_view inAttribName) const
{
  const auto attrib_location = GL::GetAttribLocation(GetGLId(), inAttribName);
  return (attrib_location != GL::InvalidId) ? std::make_optional(attrib_location) : std::nullopt;
}

std::optional<GL::Id> ShaderProgram::GetUniformLocation(const std::string_view inUniformName) const
{
  const auto uniform_location = GL::GetUniformLocation(GetGLId(), inUniformName);
  return (uniform_location != GL::InvalidId) ? std::make_optional(uniform_location) : std::nullopt;
}

std::optional<GL::Id> ShaderProgram::GetUniformBlockIndex(const std::string_view inUniformBlockName) const
{
  const auto uniform_block_index = GL::GetUniformBlockIndex(GetGLId(), inUniformBlockName);
  return (uniform_block_index != GL::InvalidId) ? std::make_optional(uniform_block_index) : std::nullopt;
}

void ShaderProgram::SetUniformBlockBinding(const std::string_view inUniformBlockName, const GL::Id inBindingPoint)
{
  const auto uniform_block_index = GetUniformBlockIndex(inUniformBlockName);
  if (!uniform_block_index.has_value())
    THROW_EXCEPTION("Uniform block with name '" << inUniformBlockName << "' does not exist in shader program with id "
                                                << GetGLId());
  SetUniformBlockBinding(*uniform_block_index, inBindingPoint);
}

void ShaderProgram::SetUniformBlockBinding(const GL::Id inUniformBlockIndex, const GL::Id inBindingPoint)
{
  EXPECTS(inUniformBlockIndex != GL::InvalidId);
  GL::UniformBlockBinding(GetGLId(), inUniformBlockIndex, inBindingPoint);
}

void ShaderProgram::SetUniformBlockBindingSafe(const std::string_view inUniformBlockName, const GL::Id inBindingPoint)
{
  const auto uniform_block_index = GetUniformBlockIndex(inUniformBlockName);
  if (!uniform_block_index.has_value())
    return;
  SetUniformBlockBinding(*uniform_block_index, inBindingPoint);
}

GL::Id ShaderProgram::GetUniformLocationWithException(const ShaderProgram& inShaderProgram,
    const std::string_view inUniformName)
{
  const auto uniform_location_optional = inShaderProgram.GetUniformLocation(inUniformName.data());
  if (!uniform_location_optional)
    THROW_EXCEPTION(
        "Uniform \"" << inUniformName.data() << "\" not found in shader program with id " << inShaderProgram.GetGLId());
  return *uniform_location_optional;
}
}