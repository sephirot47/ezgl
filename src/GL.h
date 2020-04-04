#pragma once

#include <cstdint>
#include <array>

#include <GL/glew.h>

#include "Color.h"
#include "Flags.h"
#include "Macros.h"
#include "Span.h"

namespace egl
{

class GL
{
public:
    using Uint = GLuint;
    using Int = GLint;
    using Id = GLuint;
    using Size = GLsizei;
    using Enum = GLenum;

    static constexpr auto InvalidId = static_cast<GL::Id>(-1);

    enum class EShaderType
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER
    };

    enum class EError
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

    enum class EBufferType
    {
        ARRAY_BUFFER = GL_ARRAY_BUFFER,
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER
    };

    enum class EBufferBindingType
    {
        ARRAY_BUFFER = GL_ARRAY_BUFFER_BINDING,
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER_BINDING
    };

    enum class EAccessHint
    {
        STATIC_DRAW = GL_STATIC_DRAW,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        STREAM_DRAW = GL_STREAM_DRAW,
        STATIC_READ = GL_STATIC_READ,
        DYNAMIC_READ = GL_DYNAMIC_READ,
        STREAM_READ = GL_STREAM_READ,
        STATIC_COPY = GL_STATIC_COPY,
        DYNAMIC_COPY = GL_DYNAMIC_COPY,
        STREAM_COPY = GL_STREAM_COPY
    };

    enum class EBufferBitFlags
    {
        COLOR = GL_COLOR_BUFFER_BIT,
        DEPTH = GL_DEPTH_BUFFER_BIT,
        ACCUM = GL_ACCUM_BUFFER_BIT,
        STENCIL = GL_STENCIL_BUFFER_BIT
    };

    enum class EDataType
    {
        BOOL = GL_BOOL,
        BYTE = GL_BYTE,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        SHORT = GL_SHORT,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        INT = GL_INT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        FLOAT = GL_FLOAT,
        DOUBLE = GL_DOUBLE,

        BOOL_VEC2 = GL_BOOL_VEC2,
        BOOL_VEC3 = GL_BOOL_VEC3,
        BOOL_VEC4 = GL_BOOL_VEC4,
        INT_VEC2 = GL_INT_VEC2,
        INT_VEC3 = GL_INT_VEC3,
        INT_VEC4 = GL_INT_VEC4,
        UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2,
        UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3,
        UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4,
        FLOAT_VEC2 = GL_FLOAT_VEC2,
        FLOAT_VEC3 = GL_FLOAT_VEC3,
        FLOAT_VEC4 = GL_FLOAT_VEC4,
        DOUBLE_VEC2 = GL_DOUBLE_VEC2,
        DOUBLE_VEC3 = GL_DOUBLE_VEC3,
        DOUBLE_VEC4 = GL_DOUBLE_VEC4,

        FLOAT_MAT2 = GL_FLOAT_MAT2,
        DOUBLE_MAT2 = GL_DOUBLE_MAT2,
        FLOAT_MAT3 = GL_FLOAT_MAT3,
        DOUBLE_MAT3 = GL_DOUBLE_MAT3,
        FLOAT_MAT4 = GL_FLOAT_MAT4,
        DOUBLE_MAT4 = GL_DOUBLE_MAT4,
    };

    enum class EPrimitivesMode
    {
        TRIANGLES = GL_TRIANGLES
    };

    static GL::Id GenBuffer();
    static GL::Id CreateBuffer();
    static void BindBuffer(const GL::EBufferType inBufferType, const GL::Id inBufferId);
    template <typename T>
    static void BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EAccessHint inAccessHint);
    static void DeleteBuffer(const GL::Id inBufferId);

    static void ClearColor(const Color4f& inColor);
    static void ClearBuffer(const GL::EBufferBitFlags& inBufferBitFlags);

    static void DrawElements(const GL::EPrimitivesMode inPrimitivesMode, const GL::Size inNumberOfPrimitives, const GL::EDataType inIndicesDataType, const GL::Size inBeginPrimiviteIndex = 0);

    template <typename T>
    static GL::Int GetInteger(const T& inGLEnum);

    static GL::EError CheckError();
    static void ClearError();
    static std::string ErrorToString(GL::EError inGLError);

    template <typename T>
    static GL::Enum EnumCast(const T& inGLEnum);

    GL() = delete;
};

// clang-format off
#define GL_SAFE_CALL(EXPR) [&](){ GL::ClearError(); EXPR; const auto error = GL::CheckError(); if (error != GL::EError::NO_ERROR) THROW_EXCEPTION("GL error: " << GL::ErrorToString(error)); }();
#define GL_SAFE_CALL_RET(EXPR) [&](){ GL::ClearError(); const auto ret = EXPR; const auto error = GL::CheckError(); if (error != GL::EError::NO_ERROR) THROW_EXCEPTION("GL error: " << GL::ErrorToString(error)); return ret; }();
// clang-format on

DECLARE_FLAGS(GL::EBufferBitFlags);

}

#include "GL.tcc"