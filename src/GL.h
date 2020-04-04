#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <GL/glew.h>

#include "Color.h"
#include "Flags.h"
#include "Macros.h"
#include "Mat.h"
#include "Span.h"
#include "Vec.h"

namespace egl
{

class GL
{
public:
    using Char = GLchar;
    using Byte = GLbyte;
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
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
        VERTEX_ARRAY = GL_VERTEX_ARRAY
    };

    enum class EBufferBindingType
    {
        ARRAY_BUFFER = GL_ARRAY_BUFFER_BINDING,
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER_BINDING,
        VERTEX_ARRAY = GL_VERTEX_ARRAY_BINDING
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

    enum class EShaderInfo
    {
        COMPILE_STATUS = GL_COMPILE_STATUS,
        INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH
    };

    enum class EShaderProgramEnum
    {
        CURRENT_PROGRAM = GL_CURRENT_PROGRAM
    };

    static GL::Id GenBuffer();
    static GL::Id CreateBuffer();
    static void BindBuffer(const GL::EBufferType inBufferType, const GL::Id inBufferId);
    template <typename T>
    static void BufferData(const GL::Id inBufferId, const Span<T>& inData, const GL::EAccessHint inAccessHint);
    static void DeleteBuffer(const GL::Id inBufferId);

    static GL::Id GenVertexArray();
    static GL::Id CreateVertexArray();
    static void BindVertexArray(const GL::Id inVAOId);
    static void EnableVertexAttribArray(const GL::Id inAttribLocation);
    static void VertexAttribPointer(const GL::Id inAttribLocation, const GL::Size inNumComponents, const GL::EDataType inDataType, const bool inNormalized, const GL::Size inStride, const GL::Size inOffset = 0);
    static void DisableVertexAttribArray(const GL::Id inAttribLocation);
    static void DeleteVertexArray(const GL::Id inVAOId);

    static void ClearColor(const Color4f& inColor);
    static void ClearBuffer(const GL::EBufferBitFlags& inBufferBitFlags);
    static void DrawElements(const GL::EPrimitivesMode inPrimitivesMode, const GL::Size inNumberOfPrimitives, const GL::EDataType inIndicesDataType, const GL::Size inBeginPrimiviteIndex = 0);

    static GL::Id CreateShader(const GL::EShaderType inShaderType);
    static void ShaderSource(const GL::Id inShaderId, const std::string_view inSourceCode);
    static void CompileShader(const GL::Id inShaderId);
    static GL::Int GetShaderInteger(const GL::Id inShaderId, const GL::EShaderInfo inShaderInfoEnum);
    static std::string GetShaderInfoLog(const GL::Id inShaderId);
    static void DeleteShader(const GL::Id inShaderId);

    static GL::Id CreateProgram();
    static void UseProgram(const GL::Id inShaderProgramId);
    static void AttachShader(const GL::Id inShaderProgramId, const GL::Id inShaderId);
    static void LinkProgram(const GL::Id inShaderProgramId);
    static GL::Id GetAttribLocation(const GL::Id inShaderProgramId, const std::string_view inAttribName);
    static GL::Id GetUniformLocation(const GL::Id inShaderProgramId, const std::string_view inUniformName);
    static void Uniform(const GL::Id inUniformLocation, bool inValue);
    static void Uniform(const GL::Id inUniformLocation, int8_t inValue);
    static void Uniform(const GL::Id inUniformLocation, int16_t inValue);
    static void Uniform(const GL::Id inUniformLocation, int32_t inValue);
    static void Uniform(const GL::Id inUniformLocation, float inValue);
    static void Uniform(const GL::Id inUniformLocation, double inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec2i& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec2f& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec2d& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec3i& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec3f& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec3d& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec4i& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec4f& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Vec4d& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Mat2f& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Mat2d& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Mat3f& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Mat3d& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Mat4f& inValue);
    static void Uniform(const GL::Id inUniformLocation, const Mat4d& inValue);
    static void DeleteProgram(const GL::Id inShaderProgramId);

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