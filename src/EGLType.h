#pragma once

#include <GL/glew.h>

#include "GL.h"
#include "Mat.h"
#include "Vec.h"

namespace egl
{
template <typename T, uint8_t TNumComponents = 1>
struct _GLTypeTraitsBase
{
    using ComponentType = T;
    static constexpr auto NumComponents = TNumComponents;
    static constexpr auto NumBytes = NumComponents * sizeof(T);
};

template <typename T>
struct GLTypeTraits : public _GLTypeTraitsBase<bool>
{
};

template <>
struct GLTypeTraits<bool> : public _GLTypeTraitsBase<bool>
{
    static constexpr auto GLType = GL::EDataType::BOOL;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<int8_t> : public _GLTypeTraitsBase<int8_t>
{
    static constexpr auto GLType = GL::EDataType::INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<uint8_t> : public _GLTypeTraitsBase<uint8_t>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<int16_t> : public _GLTypeTraitsBase<int16_t>
{
    static constexpr auto GLType = GL::EDataType::SHORT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<uint16_t> : public _GLTypeTraitsBase<uint16_t>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_SHORT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<int32_t> : public _GLTypeTraitsBase<int32_t>
{
    static constexpr auto GLType = GL::EDataType::INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<uint32_t> : public _GLTypeTraitsBase<uint32_t>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<float> : public _GLTypeTraitsBase<float>
{
    static constexpr auto GLType = GL::EDataType::FLOAT;
    static constexpr auto GLComponentType = GLType;
};

template <>
struct GLTypeTraits<double> : public _GLTypeTraitsBase<double>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE;
    static constexpr auto GLComponentType = GLType;
};

template <typename, std::size_t>
class Vec;

template <>
struct GLTypeTraits<Vec<bool, 2u>> : public _GLTypeTraitsBase<bool, 2u>
{
    static constexpr auto GLType = GL::EDataType::BOOL_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::BOOL;
};

template <>
struct GLTypeTraits<Vec<bool, 3u>> : public _GLTypeTraitsBase<bool, 3u>
{
    static constexpr auto GLType = GL::EDataType::BOOL_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::BOOL;
};

template <>
struct GLTypeTraits<Vec<bool, 4u>> : public _GLTypeTraitsBase<bool, 4u>
{
    static constexpr auto GLType = GL::EDataType::BOOL_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::BOOL;
};

template <>
struct GLTypeTraits<Vec<int32_t, 2u>> : public _GLTypeTraitsBase<int32_t, 2u>
{
    static constexpr auto GLType = GL::EDataType::INT_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::INT;
};

template <>
struct GLTypeTraits<Vec<int32_t, 3u>> : public _GLTypeTraitsBase<int32_t, 3u>
{
    static constexpr auto GLType = GL::EDataType::INT_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::INT;
};

template <>
struct GLTypeTraits<Vec<int32_t, 4u>> : public _GLTypeTraitsBase<int32_t, 4u>
{
    static constexpr auto GLType = GL::EDataType::INT_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::INT;
};

template <>
struct GLTypeTraits<Vec<uint32_t, 2u>> : public _GLTypeTraitsBase<uint32_t, 2u>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<Vec<uint32_t, 3u>> : public _GLTypeTraitsBase<uint32_t, 3u>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<Vec<uint32_t, 4u>> : public _GLTypeTraitsBase<uint32_t, 4u>
{
    static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<Vec<float, 2u>> : public _GLTypeTraitsBase<float, 2u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Vec<float, 3u>> : public _GLTypeTraitsBase<float, 3u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Vec<float, 4u>> : public _GLTypeTraitsBase<float, 4u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Vec<double, 2u>> : public _GLTypeTraitsBase<double, 2u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_VEC2;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Vec<double, 3u>> : public _GLTypeTraitsBase<double, 3u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_VEC3;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Vec<double, 4u>> : public _GLTypeTraitsBase<double, 4u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_VEC4;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Mat2f> : public _GLTypeTraitsBase<float, 2u * 2u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_MAT2;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Mat2d> : public _GLTypeTraitsBase<double, 2u * 2u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_MAT2;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Mat3f> : public _GLTypeTraitsBase<float, 3u * 3u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_MAT3;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Mat3d> : public _GLTypeTraitsBase<double, 3u * 3u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_MAT3;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Mat4f> : public _GLTypeTraitsBase<float, 4u * 4u>
{
    static constexpr auto GLType = GL::EDataType::FLOAT_MAT4;
    static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Mat4d> : public _GLTypeTraitsBase<double, 4u * 4u>
{
    static constexpr auto GLType = GL::EDataType::DOUBLE_MAT4;
    static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};
}