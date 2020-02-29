#pragma once

#include <GL/glew.h>

namespace egl
{
enum class EGLType
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
};

template <typename T>
struct EGLTypeTraits
{
    static constexpr auto Size = sizeof(T);
};

template <typename T>
using EGLTypeTraits_t = typename EGLTypeTraits<T>::Type;

template <>
struct EGLTypeTraits<bool>
{
    static constexpr auto Type = EGLType::BOOL;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<int8_t>
{
    static constexpr auto Type = EGLType::INT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<uint8_t>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<int16_t>
{
    static constexpr auto Type = EGLType::SHORT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<uint16_t>
{
    static constexpr auto Type = EGLType::UNSIGNED_SHORT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<int32_t>
{
    static constexpr auto Type = EGLType::INT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<uint32_t>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<float>
{
    static constexpr auto Type = EGLType::FLOAT;
    static constexpr auto NumComponents = 1;
};

template <>
struct EGLTypeTraits<double>
{
    static constexpr auto Type = EGLType::DOUBLE;
    static constexpr auto NumComponents = 1;
};
}