#pragma once

#include <GL/glew.h>

#include "Macros.h"

namespace egl
{
using GLId = GLuint;

enum class EShaderType
{
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
};

enum class EGLError
{
    NO_ERROR = GL_NO_ERROR,
    INVALID_ENUM = GL_INVALID_ENUM,
    INVALID_VALUE = GL_INVALID_VALUE,
    INVALID_OPERATION = GL_INVALID_OPERATION,
    INVALID_FRAMEBUFFER_OPERATION = GL_INVALID_FRAMEBUFFER_OPERATION,
    OUT_OF_MEMORY = GL_OUT_OF_MEMORY,
    STACK_UNDERFLOW = GL_STACK_UNDERFLOW,
    STACK_OVERFLOW = GL_STACK_OVERFLOW
};
EGLError GLCheckError();
void GLClearError();
std::string GLErrorToString(EGLError inGLError);

// clang-format off
#define GL_SAFE_CALL(EXPR) [&](){ GLClearError(); EXPR; const auto error = GLCheckError(); if (error != EGLError::NO_ERROR) THROW_EXCEPTION("GL error: " << GLErrorToString(error)); }();
#define GL_SAFE_CALL_RET(EXPR) [&](){ GLClearError(); const auto ret = EXPR; const auto error = GLCheckError(); if (error != EGLError::NO_ERROR) THROW_EXCEPTION("GL error: " << GLErrorToString(error)); return ret; }();
// clang-format on

}