#include "GL.h"

namespace egl
{
EGLError GLCheckError()
{
    return static_cast<EGLError>(glGetError());
}

void GLClearError()
{
    while (GLCheckError() != EGLError::NO_ERROR)
    {
    }
}

std::string GLErrorToString(EGLError inGLError)
{
    switch (inGLError)
    {
    case EGLError::NO_ERROR:
        return "NO_ERROR";
    case EGLError::INVALID_ENUM:
        return "INVALID_ENUM";
    case EGLError::INVALID_VALUE:
        return "INVALID_VALUE";
    case EGLError::INVALID_OPERATION:
        return "INVALID_OPERATION";
    case EGLError::INVALID_FRAMEBUFFER_OPERATION:
        return "INVALID_FRAMEBUFFER_OPERATION";
    case EGLError::OUT_OF_MEMORY:
        return "OUT_OF_MEMORY";
    case EGLError::STACK_UNDERFLOW:
        return "STACK_UNDERFLOW";
    case EGLError::STACK_OVERFLOW:
        return "STACK_OVERFLOW";
    }
    return "UNKNOWN_ERROR";
}
}