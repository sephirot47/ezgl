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