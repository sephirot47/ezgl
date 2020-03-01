#pragma once

#include <GL/glew.h>
#include "Vec.h"

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
};

template <typename T, uint8_t TNumComponents = 1>
struct _EGLTypeTraitsBase
{
    using ComponentType = T;
    static constexpr auto NumComponents = 1;
    static constexpr auto NumBytes = NumComponents * sizeof(T);
};

template <typename T>
struct EGLTypeTraits : public _EGLTypeTraitsBase<bool>
{
};

template <>
struct EGLTypeTraits<bool> : public _EGLTypeTraitsBase<bool>
{
    static constexpr auto Type = EGLType::BOOL;
};

template <>
struct EGLTypeTraits<int8_t> : public _EGLTypeTraitsBase<int8_t>
{
    static constexpr auto Type = EGLType::INT;
};

template <>
struct EGLTypeTraits<uint8_t> : public _EGLTypeTraitsBase<uint8_t>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<int16_t> : public _EGLTypeTraitsBase<int16_t>
{
    static constexpr auto Type = EGLType::SHORT;
};

template <>
struct EGLTypeTraits<uint16_t> : public _EGLTypeTraitsBase<uint16_t>
{
    static constexpr auto Type = EGLType::UNSIGNED_SHORT;
};

template <>
struct EGLTypeTraits<int32_t> : public _EGLTypeTraitsBase<int32_t>
{
    static constexpr auto Type = EGLType::INT;
};

template <>
struct EGLTypeTraits<uint32_t> : public _EGLTypeTraitsBase<uint32_t>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<float> : public _EGLTypeTraitsBase<float>
{
    static constexpr auto Type = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<double> : public _EGLTypeTraitsBase<double>
{
    static constexpr auto Type = EGLType::DOUBLE;
};

template <typename, uint8_t>
class Vec;

template <>
struct EGLTypeTraits<Vec<bool, 2u> > : public _EGLTypeTraitsBase<bool, 2u>
{
    static constexpr auto Type = EGLType::BOOL_VEC2;
};

template <>
struct EGLTypeTraits<Vec<bool, 3u> > : public _EGLTypeTraitsBase<bool, 3u>
{
    static constexpr auto Type = EGLType::BOOL_VEC3;
};

template <>
struct EGLTypeTraits<Vec<bool, 4u> > : public _EGLTypeTraitsBase<bool, 4u>
{
    static constexpr auto Type = EGLType::BOOL_VEC4;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 2u> > : public _EGLTypeTraitsBase<int32_t, 2u>
{
    static constexpr auto Type = EGLType::INT_VEC2;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 3u> > : public _EGLTypeTraitsBase<int32_t, 3u>
{
    static constexpr auto Type = EGLType::INT_VEC3;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 4u> > : public _EGLTypeTraitsBase<int32_t, 4u>
{
    static constexpr auto Type = EGLType::INT_VEC4;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 2u> > : public _EGLTypeTraitsBase<uint32_t, 2u>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT_VEC2;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 3u> > : public _EGLTypeTraitsBase<uint32_t, 3u>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT_VEC3;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 4u> > : public _EGLTypeTraitsBase<uint32_t, 4u>
{
    static constexpr auto Type = EGLType::UNSIGNED_INT_VEC4;
};

template <>
struct EGLTypeTraits<Vec<float, 2u> > : public _EGLTypeTraitsBase<float, 2u>
{
    static constexpr auto Type = EGLType::FLOAT_VEC2;
};

template <>
struct EGLTypeTraits<Vec<float, 3u> > : public _EGLTypeTraitsBase<float, 3u>
{
    static constexpr auto Type = EGLType::FLOAT_VEC3;
};

template <>
struct EGLTypeTraits<Vec<float, 4u> > : public _EGLTypeTraitsBase<float, 4u>
{
    static constexpr auto Type = EGLType::FLOAT_VEC4;
};

template <>
struct EGLTypeTraits<Vec<double, 2u> > : public _EGLTypeTraitsBase<double, 2u>
{
    static constexpr auto Type = EGLType::DOUBLE_VEC2;
};

template <>
struct EGLTypeTraits<Vec<double, 3u> > : public _EGLTypeTraitsBase<double, 3u>
{
    static constexpr auto Type = EGLType::DOUBLE_VEC3;
};

template <>
struct EGLTypeTraits<Vec<double, 4u> > : public _EGLTypeTraitsBase<double, 4u>
{
    static constexpr auto Type = EGLType::DOUBLE_VEC4;
};
}