#pragma once

#include <GL/glew.h>
#include "Mat.h"
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

    FLOAT_MAT2 = GL_FLOAT_MAT2,
    DOUBLE_MAT2 = GL_DOUBLE_MAT2,
    FLOAT_MAT3 = GL_FLOAT_MAT3,
    DOUBLE_MAT3 = GL_DOUBLE_MAT3,
    FLOAT_MAT4 = GL_FLOAT_MAT4,
    DOUBLE_MAT4 = GL_DOUBLE_MAT4,
};

template <typename T, uint8_t TNumComponents = 1>
struct _EGLTypeTraitsBase
{
    using ComponentType = T;
    static constexpr auto NumComponents = TNumComponents;
    static constexpr auto NumBytes = NumComponents * sizeof(T);
};

template <typename T>
struct EGLTypeTraits : public _EGLTypeTraitsBase<bool>
{
};

template <>
struct EGLTypeTraits<bool> : public _EGLTypeTraitsBase<bool>
{
    static constexpr auto GLType = EGLType::BOOL;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<int8_t> : public _EGLTypeTraitsBase<int8_t>
{
    static constexpr auto GLType = EGLType::INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<uint8_t> : public _EGLTypeTraitsBase<uint8_t>
{
    static constexpr auto GLType = EGLType::UNSIGNED_INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<int16_t> : public _EGLTypeTraitsBase<int16_t>
{
    static constexpr auto GLType = EGLType::SHORT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<uint16_t> : public _EGLTypeTraitsBase<uint16_t>
{
    static constexpr auto GLType = EGLType::UNSIGNED_SHORT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<int32_t> : public _EGLTypeTraitsBase<int32_t>
{
    static constexpr auto GLType = EGLType::INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<uint32_t> : public _EGLTypeTraitsBase<uint32_t>
{
    static constexpr auto GLType = EGLType::UNSIGNED_INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<float> : public _EGLTypeTraitsBase<float>
{
    static constexpr auto GLType = EGLType::FLOAT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<double> : public _EGLTypeTraitsBase<double>
{
    static constexpr auto GLType = EGLType::DOUBLE;
    static constexpr auto GLComponentType = GLType;
};

template <typename, std::size_t>
class Vec;

template <>
struct EGLTypeTraits<Vec<bool, 2u>> : public _EGLTypeTraitsBase<bool, 2u>
{
    static constexpr auto GLType = EGLType::BOOL_VEC2;
    static constexpr auto GLComponentType = EGLType::BOOL;
};

template <>
struct EGLTypeTraits<Vec<bool, 3u>> : public _EGLTypeTraitsBase<bool, 3u>
{
    static constexpr auto GLType = EGLType::BOOL_VEC3;
    static constexpr auto GLComponentType = EGLType::BOOL;
};

template <>
struct EGLTypeTraits<Vec<bool, 4u>> : public _EGLTypeTraitsBase<bool, 4u>
{
    static constexpr auto GLType = EGLType::BOOL_VEC4;
    static constexpr auto GLComponentType = EGLType::BOOL;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 2u>> : public _EGLTypeTraitsBase<int32_t, 2u>
{
    static constexpr auto GLType = EGLType::INT_VEC2;
    static constexpr auto GLComponentType = EGLType::INT;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 3u>> : public _EGLTypeTraitsBase<int32_t, 3u>
{
    static constexpr auto GLType = EGLType::INT_VEC3;
    static constexpr auto GLComponentType = EGLType::INT;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 4u>> : public _EGLTypeTraitsBase<int32_t, 4u>
{
    static constexpr auto GLType = EGLType::INT_VEC4;
    static constexpr auto GLComponentType = EGLType::INT;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 2u>> : public _EGLTypeTraitsBase<uint32_t, 2u>
{
    static constexpr auto GLType = EGLType::UNSIGNED_INT_VEC2;
    static constexpr auto GLComponentType = EGLType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 3u>> : public _EGLTypeTraitsBase<uint32_t, 3u>
{
    static constexpr auto GLType = EGLType::UNSIGNED_INT_VEC3;
    static constexpr auto GLComponentType = EGLType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 4u>> : public _EGLTypeTraitsBase<uint32_t, 4u>
{
    static constexpr auto GLType = EGLType::UNSIGNED_INT_VEC4;
    static constexpr auto GLComponentType = EGLType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<Vec<float, 2u>> : public _EGLTypeTraitsBase<float, 2u>
{
    static constexpr auto GLType = EGLType::FLOAT_VEC2;
    static constexpr auto GLComponentType = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<Vec<float, 3u>> : public _EGLTypeTraitsBase<float, 3u>
{
    static constexpr auto GLType = EGLType::FLOAT_VEC3;
    static constexpr auto GLComponentType = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<Vec<float, 4u>> : public _EGLTypeTraitsBase<float, 4u>
{
    static constexpr auto GLType = EGLType::FLOAT_VEC4;
    static constexpr auto GLComponentType = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<Vec<double, 2u>> : public _EGLTypeTraitsBase<double, 2u>
{
    static constexpr auto GLType = EGLType::DOUBLE_VEC2;
    static constexpr auto GLComponentType = EGLType::DOUBLE;
};

template <>
struct EGLTypeTraits<Vec<double, 3u>> : public _EGLTypeTraitsBase<double, 3u>
{
    static constexpr auto GLType = EGLType::DOUBLE_VEC3;
    static constexpr auto GLComponentType = EGLType::DOUBLE;
};

template <>
struct EGLTypeTraits<Vec<double, 4u>> : public _EGLTypeTraitsBase<double, 4u>
{
    static constexpr auto GLType = EGLType::DOUBLE_VEC4;
    static constexpr auto GLComponentType = EGLType::DOUBLE;
};

template <>
struct EGLTypeTraits<Mat2f> : public _EGLTypeTraitsBase<float, 2u * 2u>
{
    static constexpr auto GLType = EGLType::FLOAT_MAT2;
    static constexpr auto GLComponentType = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<Mat2d> : public _EGLTypeTraitsBase<double, 2u * 2u>
{
    static constexpr auto GLType = EGLType::DOUBLE_MAT2;
    static constexpr auto GLComponentType = EGLType::DOUBLE;
};

template <>
struct EGLTypeTraits<Mat3f> : public _EGLTypeTraitsBase<float, 3u * 3u>
{
    static constexpr auto GLType = EGLType::FLOAT_MAT3;
    static constexpr auto GLComponentType = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<Mat3d> : public _EGLTypeTraitsBase<double, 3u * 3u>
{
    static constexpr auto GLType = EGLType::DOUBLE_MAT3;
    static constexpr auto GLComponentType = EGLType::DOUBLE;
};

template <>
struct EGLTypeTraits<Mat4f> : public _EGLTypeTraitsBase<float, 4u * 4u>
{
    static constexpr auto GLType = EGLType::FLOAT_MAT4;
    static constexpr auto GLComponentType = EGLType::FLOAT;
};

template <>
struct EGLTypeTraits<Mat4d> : public _EGLTypeTraitsBase<double, 4u * 4u>
{
    static constexpr auto GLType = EGLType::DOUBLE_MAT4;
    static constexpr auto GLComponentType = EGLType::DOUBLE;
};
}