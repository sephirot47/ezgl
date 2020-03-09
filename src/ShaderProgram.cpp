#include "ShaderProgram.h"

#include <cassert>
#include <GL/glew.h>
#include <stdexcept>

#include "Macros.h"
#include "Shader.h"

namespace egl
{
ShaderProgram::ShaderProgram(const VertexShader& inVertexShader, const FragmentShader& inFragmentShader)
{
    mGLId = GL_SAFE_CALL_RET(glCreateProgram());
    ENSURES(mGLId > 0);

    GL_SAFE_CALL(glAttachShader(mGLId, inVertexShader.GetGLId()));
    GL_SAFE_CALL(glAttachShader(mGLId, inFragmentShader.GetGLId()));

    GL_SAFE_CALL(glLinkProgram(mGLId));
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mGLId);
}

void ShaderProgram::Bind() const
{
    GL_SAFE_CALL(glUseProgram(mGLId));
}

void ShaderProgram::UnBind() const
{
    GL_SAFE_CALL(glUseProgram(0));
}

bool ShaderProgram::IsBound() const
{
    const auto bound_id = GetBoundGLId();
    return bound_id != 0 && bound_id == mGLId;
}

GLId ShaderProgram::GetBoundGLId()
{
    GLint bound_id = 0;
    GL_SAFE_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &bound_id));
    return static_cast<GLId>(bound_id);
}

GLId GetUniformLocationWithException(const ShaderProgram& inShaderProgram, std::string_view inName)
{
    const auto uniform_location_optional = inShaderProgram.GetUniformLocation(inName.data());
    if (!uniform_location_optional)
        THROW_EXCEPTION("Uniform " << inName.data() << " not found in shader program with id " << inShaderProgram.GetGLId());
    return *uniform_location_optional;
}

void ShaderProgram::SetUniform(std::string_view inName, bool inValue)
{
    SetUniform(inName, static_cast<int32_t>(inValue));
}

void ShaderProgram::SetUniform(std::string_view inName, int8_t inValue)
{
    SetUniform(inName, static_cast<int32_t>(inValue));
}

void ShaderProgram::SetUniform(std::string_view inName, int16_t inValue)
{
    SetUniform(inName, static_cast<int32_t>(inValue));
}

void ShaderProgram::SetUniform(std::string_view inName, int32_t inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform1i(GetUniformLocationWithException(*this, inName), inValue));
}

void ShaderProgram::SetUniform(std::string_view inName, float inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform1f(GetUniformLocationWithException(*this, inName), inValue));
}

void ShaderProgram::SetUniform(std::string_view inName, double inValue)
{
    SetUniform(inName, static_cast<float>(inValue));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec2i& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform2i(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec2f& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform2f(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec2d& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform2d(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec3i& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform3i(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1], inValue[2]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec3f& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform3f(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1], inValue[2]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec3d& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform3d(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1], inValue[2]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec4i& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform4i(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1], inValue[2], inValue[3]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec4f& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform4f(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1], inValue[2], inValue[3]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Vec4d& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniform4d(GetUniformLocationWithException(*this, inName), inValue[0], inValue[1], inValue[2], inValue[3]));
}

void ShaderProgram::SetUniform(std::string_view inName, const Mat2f& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniformMatrix2fv(GetUniformLocationWithException(*this, inName), 1, true, inValue.Data()));
}

void ShaderProgram::SetUniform(std::string_view inName, const Mat2d& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniformMatrix2dv(GetUniformLocationWithException(*this, inName), 1, true, inValue.Data()));
}

void ShaderProgram::SetUniform(std::string_view inName, const Mat3f& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniformMatrix3fv(GetUniformLocationWithException(*this, inName), 1, true, inValue.Data()));
}

void ShaderProgram::SetUniform(std::string_view inName, const Mat3d& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniformMatrix3dv(GetUniformLocationWithException(*this, inName), 1, true, inValue.Data()));
}

void ShaderProgram::SetUniform(std::string_view inName, const Mat4f& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniformMatrix4fv(GetUniformLocationWithException(*this, inName), 1, true, inValue.Data()));
}

void ShaderProgram::SetUniform(std::string_view inName, const Mat4d& inValue)
{
    EXPECTS(IsBound());
    GL_SAFE_CALL(glUniformMatrix4dv(GetUniformLocationWithException(*this, inName), 1, true, inValue.Data()));
}

std::optional<GLId> ShaderProgram::GetAttribLocation(std::string_view inName) const
{
    const auto attrib_location = GL_SAFE_CALL_RET(glGetAttribLocation(mGLId, inName.data()));
    return std::make_optional(attrib_location);
}

std::optional<GLId> ShaderProgram::GetUniformLocation(std::string_view inName) const
{
    const auto uniform_location = GL_SAFE_CALL_RET(glGetUniformLocation(mGLId, inName.data()));
    return std::make_optional(uniform_location);
}
}