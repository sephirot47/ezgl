#pragma once

#include <GL/glew.h>

#include "GL.h"
#include "Mat.h"
#include "Vec.h"

namespace egl
{
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
    static constexpr auto GLType = GL::EDataType::BOOL;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<int8_t> : public _EGLTypeTraitsBase<int8_t>
{
    static constexpr auto GLType = GL::EDataType::INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<uint8_t> : public _EGLTypeTraitsBase<uint8_t>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<int16_t> : public _EGLTypeTraitsBase<int16_t>
{
    static constexpr auto GLType = GL::EDataType::SHORT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<uint16_t> : public _EGLTypeTraitsBase<uint16_t>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_SHORT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<int32_t> : public _EGLTypeTraitsBase<int32_t>
{
    static constexpr auto GLType = GL::EDataType::INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<uint32_t> : public _EGLTypeTraitsBase<uint32_t>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<float> : public _EGLTypeTraitsBase<float>
{
    static constexpr auto GLType = GL::EDataType::FLOAT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct EGLTypeTraits<double> : public _EGLTypeTraitsBase<double>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE;
    static constexpr auto GLComponentType = GLType;
};

template <typename, std::size_t>
class Vec;

template <>
struct EGLTypeTraits<Vec<bool, 2u>> : public _EGLTypeTraitsBase<bool, 2u>
{
    static constexpr auto GLType = GL::EDataType::BOOL_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::BOOL;
};

template <>
struct EGLTypeTraits<Vec<bool, 3u>> : public _EGLTypeTraitsBase<bool, 3u>
{
    static constexpr auto GLType = GL::EDataType::BOOL_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::BOOL;
};

template <>
struct EGLTypeTraits<Vec<bool, 4u>> : public _EGLTypeTraitsBase<bool, 4u>
{
    static constexpr auto GLType = GL::EDataType::BOOL_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::BOOL;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 2u>> : public _EGLTypeTraitsBase<int32_t, 2u>
{
    static constexpr auto GLType = GL::EDataType::INT_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::INT;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 3u>> : public _EGLTypeTraitsBase<int32_t, 3u>
{
    static constexpr auto GLType = GL::EDataType::INT_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::INT;
};

template <>
struct EGLTypeTraits<Vec<int32_t, 4u>> : public _EGLTypeTraitsBase<int32_t, 4u>
{
    static constexpr auto GLType = GL::EDataType::INT_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::INT;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 2u>> : public _EGLTypeTraitsBase<uint32_t, 2u>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 3u>> : public _EGLTypeTraitsBase<uint32_t, 3u>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<Vec<uint32_t, 4u>> : public _EGLTypeTraitsBase<uint32_t, 4u>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
};

template <>
struct EGLTypeTraits<Vec<float, 2u>> : public _EGLTypeTraitsBase<float, 2u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct EGLTypeTraits<Vec<float, 3u>> : public _EGLTypeTraitsBase<float, 3u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct EGLTypeTraits<Vec<float, 4u>> : public _EGLTypeTraitsBase<float, 4u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct EGLTypeTraits<Vec<double, 2u>> : public _EGLTypeTraitsBase<double, 2u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct EGLTypeTraits<Vec<double, 3u>> : public _EGLTypeTraitsBase<double, 3u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct EGLTypeTraits<Vec<double, 4u>> : public _EGLTypeTraitsBase<double, 4u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct EGLTypeTraits<Mat2f> : public _EGLTypeTraitsBase<float, 2u * 2u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_MAT2;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct EGLTypeTraits<Mat2d> : public _EGLTypeTraitsBase<double, 2u * 2u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_MAT2;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct EGLTypeTraits<Mat3f> : public _EGLTypeTraitsBase<float, 3u * 3u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_MAT3;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct EGLTypeTraits<Mat3d> : public _EGLTypeTraitsBase<double, 3u * 3u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_MAT3;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct EGLTypeTraits<Mat4f> : public _EGLTypeTraitsBase<float, 4u * 4u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_MAT4;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct EGLTypeTraits<Mat4d> : public _EGLTypeTraitsBase<double, 4u * 4u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_MAT4;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};
}