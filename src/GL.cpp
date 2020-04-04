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