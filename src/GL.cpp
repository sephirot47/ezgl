#include "GL.h"

namespace egl
{

GL::Id GL::GenBuffer()
{
    GL::Id new_buffer_id = 0;
    GL_SAFE_CALL(glGenBuffers(1, &new_buffer_id));
    return new_buffer_id;
}

GL::Id GL::CreateBuffer()
{
    GL::Id new_buffer_id = 0;
    GL_SAFE_CALL(glCreateBuffers(1, &new_buffer_id));
    return new_buffer_id;
}

void GL::BindBuffer(const GL::EBufferType inBufferType, const GL::Id inBufferId)
{
    GL_SAFE_CALL(glBindBuffer(GL::EnumCast(inBufferType), inBufferId));
}

void GL::DeleteBuffer(const GL::Id inBufferId)
{
    GL_SAFE_CALL(glDeleteBuffers(1, &inBufferId));
}

void GL::ClearColor(const Color4f& inColor)
{
    GL_SAFE_CALL(glClearColor(inColor[0], inColor[1], inColor[2], inColor[3]));
}

void GL::ClearBuffer(const GL::EBufferBitFlags& inBufferBitFlags)
{
    GL_SAFE_CALL(glClear(GL::EnumCast(inBufferBitFlags)));
}

void GL::DrawElements(const GL::EPrimitivesMode inPrimitivesMode, const GL::Size inNumberOfPrimitives, const GL::EDataType inIndicesDataType, const GL::Size inBeginPrimiviteIndex)
{
    GL_SAFE_CALL(glDrawElements(GL::EnumCast(inPrimitivesMode), inNumberOfPrimitives, GL::EnumCast(inIndicesDataType), reinterpret_cast<const void*>(inBeginPrimiviteIndex)));
}

GL::Id GL::CreateShader(const GL::EShaderType inShaderType)
{
    const auto new_shader_id = GL_SAFE_CALL_RET(glCreateShader(GL::EnumCast(inShaderType)));
    return new_shader_id;
}

void GL::ShaderSource(const GL::Id inShaderId, const std::string_view inSourceCode)
{
    const GLchar* source_code_ptr = inSourceCode.data();
    const GLint source_code_length = inSourceCode.size();
    GL_SAFE_CALL(glShaderSource(inShaderId, 1, &source_code_ptr, &source_code_length));
}

void GL::CompileShader(const GL::Id inShaderId)
{
    GL_SAFE_CALL(glCompileShader(inShaderId));
}

GL::Int GL::GetShaderInteger(const GL::Id inShaderId, const GL::EShaderInfo inShaderInfoEnum)
{
    GL::Int result = 0;
    GL_SAFE_CALL(glGetShaderiv(inShaderId, GL::EnumCast(inShaderInfoEnum), &result));
    return result;
}

std::string GL::GetShaderInfoLog(const GL::Id inShaderId)
{
    const auto max_error_length = GL::GetShaderInteger(inShaderId, GL::EShaderInfo::INFO_LOG_LENGTH);

    std::string info_msg;
    info_msg.resize(max_error_length);

    GL::Int error_length = 0;
    GL_SAFE_CALL(glGetShaderInfoLog(inShaderId, max_error_length, &error_length, reinterpret_cast<GL::Char*>(info_msg.data())));
    info_msg.resize(error_length - 1);

    return info_msg;
}

void GL::DeleteShader(const GL::Id inShaderId)
{
    GL_SAFE_CALL(glDeleteShader(inShaderId));
}

GL::Id GL::CreateProgram()
{
    return GL_SAFE_CALL_RET(glCreateProgram());
}

void GL::UseProgram(const GL::Id inShaderProgramId)
{
    GL_SAFE_CALL(glUseProgram(inShaderProgramId));
}

void GL::AttachShader(const GL::Id inShaderProgramId, const GL::Id inShaderId)
{
    GL_SAFE_CALL(glAttachShader(inShaderProgramId, inShaderId));
}

void GL::LinkProgram(const GL::Id inShaderProgramId)
{
    GL_SAFE_CALL(glLinkProgram(inShaderProgramId));
}

GL::Id GL::GetAttribLocation(const GL::Id inShaderProgramId, const std::string_view inAttribName)
{
    return GL_SAFE_CALL_RET(glGetAttribLocation(inShaderProgramId, inAttribName.data()));
}

GL::Id GL::GetUniformLocation(const GL::Id inShaderProgramId, const std::string_view inUniformName)
{
    return GL_SAFE_CALL_RET(glGetUniformLocation(inShaderProgramId, inUniformName.data()));
}

void GL::DeleteProgram(const GL::Id inShaderProgramId)
{
    GL_SAFE_CALL(glDeleteProgram(inShaderProgramId));
}

void GL::Uniform(const GL::Id inUniformLocation, bool inValue)
{
    GL::Uniform(inUniformLocation, static_cast<int32_t>(inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, int8_t inValue)
{
    GL::Uniform(inUniformLocation, static_cast<int32_t>(inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, int16_t inValue)
{
    GL::Uniform(inUniformLocation, static_cast<int32_t>(inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, int32_t inValue)
{
    GL_SAFE_CALL(glUniform1i(inUniformLocation, inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, float inValue)
{
    GL_SAFE_CALL(glUniform1f(inUniformLocation, inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, double inValue)
{
    GL_SAFE_CALL(glUniform1d(inUniformLocation, inValue));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec2i& inValue)
{
    GL_SAFE_CALL(glUniform2i(inUniformLocation, inValue[0], inValue[1]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec2f& inValue)
{
    GL_SAFE_CALL(glUniform2f(inUniformLocation, inValue[0], inValue[1]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec2d& inValue)
{
    GL_SAFE_CALL(glUniform2d(inUniformLocation, inValue[0], inValue[1]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec3i& inValue)
{
    GL_SAFE_CALL(glUniform3i(inUniformLocation, inValue[0], inValue[1], inValue[2]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec3f& inValue)
{
    GL_SAFE_CALL(glUniform3f(inUniformLocation, inValue[0], inValue[1], inValue[2]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec3d& inValue)
{
    GL_SAFE_CALL(glUniform3d(inUniformLocation, inValue[0], inValue[1], inValue[2]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec4i& inValue)
{
    GL_SAFE_CALL(glUniform4i(inUniformLocation, inValue[0], inValue[1], inValue[2], inValue[3]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec4f& inValue)
{
    GL_SAFE_CALL(glUniform4f(inUniformLocation, inValue[0], inValue[1], inValue[2], inValue[3]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Vec4d& inValue)
{
    GL_SAFE_CALL(glUniform4d(inUniformLocation, inValue[0], inValue[1], inValue[2], inValue[3]));
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat2f& inValue)
{
    GL_SAFE_CALL(glUniformMatrix2fv(inUniformLocation, 1, true, inValue.Data()));
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat2d& inValue)
{
    GL_SAFE_CALL(glUniformMatrix2dv(inUniformLocation, 1, true, inValue.Data()));
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat3f& inValue)
{
    GL_SAFE_CALL(glUniformMatrix3fv(inUniformLocation, 1, true, inValue.Data()));
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat3d& inValue)
{
    GL_SAFE_CALL(glUniformMatrix3dv(inUniformLocation, 1, true, inValue.Data()));
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat4f& inValue)
{
    GL_SAFE_CALL(glUniformMatrix4fv(inUniformLocation, 1, true, inValue.Data()));
}

void GL::Uniform(const GL::Id inUniformLocation, const Mat4d& inValue)
{
    GL_SAFE_CALL(glUniformMatrix4dv(inUniformLocation, 1, true, inValue.Data()));
}

GL::EError GL::CheckError()
{
    return static_cast<GL::EError>(glGetError());
}

void GL::ClearError()
{
    while (GL::CheckError() != GL::EError::NO_ERROR)
    {
    }
}

std::string GL::ErrorToString(const GL::EError inGLError)
{
    switch (inGLError)
    {
    case GL::EError::NO_ERROR:
        return "NO_ERROR";
    case GL::EError::INVALID_ENUM:
        return "INVALID_ENUM";
    case GL::EError::INVALID_VALUE:
        return "INVALID_VALUE";
    case GL::EError::INVALID_OPERATION:
        return "INVALID_OPERATION";
    case GL::EError::INVALID_FRAMEBUFFER_OPERATION:
        return "INVALID_FRAMEBUFFER_OPERATION";
    case GL::EError::OUT_OF_MEMORY:
        return "OUT_OF_MEMORY";
    case GL::EError::STACK_UNDERFLOW:
        return "STACK_UNDERFLOW";
    case GL::EError::STACK_OVERFLOW:
        return "STACK_OVERFLOW";
    }
    return "UNKNOWN_ERROR";
}
}