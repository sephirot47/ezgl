#include "ShaderProgram.h"
#include "GL.h"
#include "Macros.h"
#include "Shader.h"
#include <GL/glew.h>
#include <cassert>
#include <stdexcept>

namespace egl
{
ShaderProgram::ShaderProgram(const VertexShader& inVertexShader, const FragmentShader& inFragmentShader)
{
  mGLId = GL::CreateProgram();
  if (mGLId == 0)
    THROW_EXCEPTION("Error creating ShaderProgram");

  GL::AttachShader(mGLId, inVertexShader.GetGLId());
  GL::AttachShader(mGLId, inFragmentShader.GetGLId());
  GL::LinkProgram(mGLId);
}

ShaderProgram::~ShaderProgram() { GL::DeleteProgram(mGLId); }

void ShaderProgram::Bind() const { GL::UseProgram(mGLId); }

void ShaderProgram::UnBind() const { GL::UseProgram(0); }

bool ShaderProgram::IsBound() const
{
  const auto bound_id = GetBoundGLId();
  return bound_id != 0 && bound_id == mGLId;
}

GL::Id ShaderProgram::GetBoundGLId()
{
  const auto bound_id = GL::GetInteger(GL::EShaderProgramEnum::CURRENT_PROGRAM);
  return static_cast<GL::Id>(bound_id);
}

std::optional<GL::Id> ShaderProgram::GetAttribLocation(const std::string_view inAttribName) const
{
  const auto attrib_location = GL::GetAttribLocation(mGLId, inAttribName);
  return (attrib_location != GL::InvalidId) ? std::make_optional(attrib_location) : std::nullopt;
}

std::optional<GL::Id> ShaderProgram::GetUniformLocation(const std::string_view inUniformName) const
{
  const auto uniform_location = GL::GetUniformLocation(mGLId, inUniformName);
  return (uniform_location != GL::InvalidId) ? std::make_optional(uniform_location) : std::nullopt;
}

std::optional<GL::Id> ShaderProgram::GetUniformBlockIndex(const std::string_view inUniformBlockName) const
{
  const auto uniform_block_index = GL::GetUniformBlockIndex(mGLId, inUniformBlockName);
  return (uniform_block_index != GL::InvalidId) ? std::make_optional(uniform_block_index) : std::nullopt;
}

void ShaderProgram::SetUniformBlockBinding(const std::string_view inUniformBlockName, const GL::Id inBindingPoint)
{
  const auto uniform_block_index = GetUniformBlockIndex(inUniformBlockName);
  if (!uniform_block_index.has_value())
    THROW_EXCEPTION(
        "Uniform block with name '" << inUniformBlockName << "' does not exist in shader program with id " << mGLId);
  SetUniformBlockBinding(*uniform_block_index, inBindingPoint);
}

void ShaderProgram::SetUniformBlockBinding(const GL::Id inUniformBlockIndex, const GL::Id inBindingPoint)
{
  EXPECTS(inUniformBlockIndex != GL::InvalidId);
  GL::UniformBlockBinding(mGLId, inUniformBlockIndex, inBindingPoint);
}

void ShaderProgram::SetUniformBlockBindingSafe(const std::string_view inUniformBlockName, const GL::Id inBindingPoint)
{
  const auto uniform_block_index = GetUniformBlockIndex(inUniformBlockName);
  if (!uniform_block_index.has_value())
    return;
  SetUniformBlockBinding(*uniform_block_index, inBindingPoint);
}

GL::Id ShaderProgram::GetGLId() const { return mGLId; }

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