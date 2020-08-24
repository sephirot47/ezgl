#pragma once

#include <ez/GL.h>
#include <ez/Mat.h>
#include <ez/Vec.h>
#include <GL/glew.h>

namespace ez
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
struct GLTypeTraits<bool> final : public _GLTypeTraitsBase<bool>
{
  static constexpr auto GLType = GL::EDataType::BOOL;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<int8_t> final : public _GLTypeTraitsBase<int8_t>
{
  static constexpr auto GLType = GL::EDataType::BYTE;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::BYTE;
};

template <>
struct GLTypeTraits<uint8_t> final : public _GLTypeTraitsBase<uint8_t>
{
  static constexpr auto GLType = GL::EDataType::UNSIGNED_BYTE;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::UNSIGNED_BYTE;
};

template <>
struct GLTypeTraits<int16_t> final : public _GLTypeTraitsBase<int16_t>
{
  static constexpr auto GLType = GL::EDataType::SHORT;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::SHORT;
};

template <>
struct GLTypeTraits<uint16_t> final : public _GLTypeTraitsBase<uint16_t>
{
  static constexpr auto GLType = GL::EDataType::UNSIGNED_SHORT;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::UNSIGNED_SHORT;
};

template <>
struct GLTypeTraits<int32_t> final : public _GLTypeTraitsBase<int32_t>
{
  static constexpr auto GLType = GL::EDataType::INT;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::INT;
};

template <>
struct GLTypeTraits<uint32_t> final : public _GLTypeTraitsBase<uint32_t>
{
  static constexpr auto GLType = GL::EDataType::UNSIGNED_INT;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<float> final : public _GLTypeTraitsBase<float>
{
  static constexpr auto GLType = GL::EDataType::FLOAT;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<double> final : public _GLTypeTraitsBase<double>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE;
  static constexpr auto GLComponentType = GLType;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RED;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RED;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::DOUBLE;
};

template <typename, std::size_t>
class Vec;

template <>
struct GLTypeTraits<Vec<bool, 2u>> final : public _GLTypeTraitsBase<bool, 2u>
{
  static constexpr auto GLType = GL::EDataType::BOOL_VEC2;
  static constexpr auto GLComponentType = GL::EDataType::BOOL;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RG;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RG;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<Vec<bool, 3u>> final : public _GLTypeTraitsBase<bool, 3u>
{
  static constexpr auto GLType = GL::EDataType::BOOL_VEC3;
  static constexpr auto GLComponentType = GL::EDataType::BOOL;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGB;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGB;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<Vec<bool, 4u>> final : public _GLTypeTraitsBase<bool, 4u>
{
  static constexpr auto GLType = GL::EDataType::BOOL_VEC4;
  static constexpr auto GLComponentType = GL::EDataType::BOOL;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGBA;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGBA;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<Vec<int32_t, 2u>> final : public _GLTypeTraitsBase<int32_t, 2u>
{
  static constexpr auto GLType = GL::EDataType::INT_VEC2;
  static constexpr auto GLComponentType = GL::EDataType::INT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RG;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RG;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::INT;
};

template <>
struct GLTypeTraits<Vec<int32_t, 3u>> final : public _GLTypeTraitsBase<int32_t, 3u>
{
  static constexpr auto GLType = GL::EDataType::INT_VEC3;
  static constexpr auto GLComponentType = GL::EDataType::INT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGB;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGB;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::INT;
};

template <>
struct GLTypeTraits<Vec<int32_t, 4u>> final : public _GLTypeTraitsBase<int32_t, 4u>
{
  static constexpr auto GLType = GL::EDataType::INT_VEC4;
  static constexpr auto GLComponentType = GL::EDataType::INT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGBA;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGBA;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::INT;
};

template <>
struct GLTypeTraits<Vec<uint32_t, 2u>> final : public _GLTypeTraitsBase<uint32_t, 2u>
{
  static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC2;
  static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RG;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RG;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<Vec<uint32_t, 3u>> final : public _GLTypeTraitsBase<uint32_t, 3u>
{
  static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC3;
  static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGB;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGB;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<Vec<uint32_t, 4u>> final : public _GLTypeTraitsBase<uint32_t, 4u>
{
  static constexpr auto GLType = GL::EDataType::UNSIGNED_INT_VEC4;
  static constexpr auto GLComponentType = GL::EDataType::UNSIGNED_INT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGBA;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGBA;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::UNSIGNED_INT;
};

template <>
struct GLTypeTraits<Vec<float, 2u>> final : public _GLTypeTraitsBase<float, 2u>
{
  static constexpr auto GLType = GL::EDataType::FLOAT_VEC2;
  static constexpr auto GLComponentType = GL::EDataType::FLOAT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RG;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RG;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<Vec<float, 3u>> final : public _GLTypeTraitsBase<float, 3u>
{
  static constexpr auto GLType = GL::EDataType::FLOAT_VEC3;
  static constexpr auto GLComponentType = GL::EDataType::FLOAT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGB;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGB;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<Vec<float, 4u>> final : public _GLTypeTraitsBase<float, 4u>
{
  static constexpr auto GLType = GL::EDataType::FLOAT_VEC4;
  static constexpr auto GLComponentType = GL::EDataType::FLOAT;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGBA;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGBA;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::FLOAT;
};

template <>
struct GLTypeTraits<Vec<double, 2u>> final : public _GLTypeTraitsBase<double, 2u>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE_VEC2;
  static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RG;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RG;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::DOUBLE;
};

template <>
struct GLTypeTraits<Vec<double, 3u>> final : public _GLTypeTraitsBase<double, 3u>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE_VEC3;
  static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGB;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGB;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::DOUBLE;
};

template <>
struct GLTypeTraits<Vec<double, 4u>> final : public _GLTypeTraitsBase<double, 4u>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE_VEC4;
  static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
  static constexpr auto GLTextureInputFormat = GL::ETextureInputFormat::RGBA;
  static constexpr auto GLTextureFormat = GL::ETextureFormat::RGBA;
  static constexpr auto GLTextureComponentFormat = GL::ETextureInputComponentFormat::DOUBLE;
};

template <>
struct GLTypeTraits<Mat2f> final : public _GLTypeTraitsBase<float, 2u * 2u>
{
  static constexpr auto GLType = GL::EDataType::FLOAT_MAT2;
  static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Mat2d> final : public _GLTypeTraitsBase<double, 2u * 2u>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE_MAT2;
  static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Mat3f> final : public _GLTypeTraitsBase<float, 3u * 3u>
{
  static constexpr auto GLType = GL::EDataType::FLOAT_MAT3;
  static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Mat3d> final : public _GLTypeTraitsBase<double, 3u * 3u>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE_MAT3;
  static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};

template <>
struct GLTypeTraits<Mat4f> final : public _GLTypeTraitsBase<float, 4u * 4u>
{
  static constexpr auto GLType = GL::EDataType::FLOAT_MAT4;
  static constexpr auto GLComponentType = GL::EDataType::FLOAT;
};

template <>
struct GLTypeTraits<Mat4d> final : public _GLTypeTraitsBase<double, 4u * 4u>
{
  static constexpr auto GLType = GL::EDataType::DOUBLE_MAT4;
  static constexpr auto GLComponentType = GL::EDataType::DOUBLE;
};
}