#include "ez/Texture.h"

namespace ez
{

template <GL::ETextureTarget TTextureTarget>
Texture<TTextureTarget>::Texture()
{
  // For texture completeness
  SetWrapMode(GL::EWrapMode::CLAMP_TO_EDGE);
  SetMinFilterMode(GL::EMinFilterMode::NEAREST);
  SetMagFilterMode(GL::EMagFilterMode::NEAREST);
}

template <GL::ETextureTarget TTextureTarget>
Texture<TTextureTarget>::Texture(const Veci<N>& inSize, const GL::ETextureFormat inFormat) : Texture()
{
  TexImageEmpty(inSize, inFormat);
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::BindToTextureUnit(const GL::Size& inTextureUnit) const
{
  EXPECTS((inTextureUnit + GL_TEXTURE0) <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  GL::BindTextureUnit(inTextureUnit, GetGLId());
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::BindImageTexture(const GL::Uint inImageUnit,
    const GL::EAccess inAccess,
    const GL::Int inLevel,
    const GL::Boolean inLayered,
    const GL::Int inLayer) const
{
  GL::BindImageTexture(inImageUnit, GetGLId(), GetFormat(), inAccess, inLevel, inLayered, inLayer);
}

template <GL::ETextureTarget TTextureTarget>
template <typename T>
void Texture<TTextureTarget>::TexImage(const Veci<N>& inSize,
    const GL::ETextureInputFormat inInputFormat,
    const GL::ETextureInputComponentFormat inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureFormat inFormat,
    const GL::Int inMipMapLevel)
{
  const auto texture_bind_guard = this->BindGuarded();

  mSize = inSize;
  mFormat = inFormat;

  if constexpr (N == 1)
  {
    GL::TexImage1D(GetTextureTarget(),
        inSize[0],
        inInputFormat,
        inInputComponentFormat,
        inData,
        inFormat,
        inMipMapLevel);
  }
  else if constexpr (N == 2)
  {
    GL::TexImage2D(GetTextureTarget(),
        inSize[0],
        inSize[1],
        inInputFormat,
        inInputComponentFormat,
        inData,
        inFormat,
        inMipMapLevel);
  }
  else if constexpr (N == 3)
  {
    GL::TexImage3D(GetTextureTarget(),
        inSize[0],
        inSize[1],
        inSize[2],
        inInputFormat,
        inInputComponentFormat,
        inData,
        inFormat,
        inMipMapLevel);
  }
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::TextureStorage(const Veci<N>& inSize,
    const GL::ETextureFormat inTextureFormat,
    const GL::Size inMipMapLevels)
{
  mSize = inSize;
  mFormat = inTextureFormat;

  if constexpr (N == 1)
  {
    GL::TextureStorage1D(GetGLId(), inTextureFormat, inSize[0], inMipMapLevels);
  }
  else if constexpr (N == 2)
  {
    GL::TextureStorage2D(GetGLId(), inTextureFormat, inSize[0], inSize[1], inMipMapLevels);
  }
  else if constexpr (N == 3)
  {
    GL::TextureStorage3D(GetGLId(), inTextureFormat, inSize[0], inSize[1], inSize[2], inMipMapLevels);
  }
}

template <GL::ETextureTarget TTextureTarget>
template <typename T>
void Texture<TTextureTarget>::TextureSubImage(const Veci<N>& inOffset,
    const Veci<N>& inSize,
    const GL::ETextureInputFormat inInputFormat,
    const GL::EDataType inInputDataType,
    const Span<T>& inData,
    const GL::Int inMipMapLevel)
{
  if constexpr (N == 1)
  {
    GL::TextureSubImage1D(this->GetGLId(),
        inOffset[0],
        inSize[0],
        inInputFormat,
        inInputDataType,
        inData,
        inMipMapLevel);
  }
  else if constexpr (N == 2)
  {
    GL::TextureSubImage2D(this->GetGLId(),
        inOffset[0],
        inOffset[1],
        inSize[0],
        inSize[1],
        inInputFormat,
        inInputDataType,
        inData,
        inMipMapLevel);
  }
  else if constexpr (N == 3)
  {
    GL::TextureSubImage3D(this->GetGLId(),
        inOffset[0],
        inOffset[1],
        inOffset[2],
        inSize[0],
        inSize[1],
        inSize[2],
        inInputFormat,
        inInputDataType,
        inData,
        inMipMapLevel);
  }
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::CopySubData(const Veci<N>& inSourcePosition,
    const Veci<N>& inSourceSize,
    Texture<TTextureTarget>& ioDestinationTexture,
    const Veci<N>& inDestinationPosition,
    const GL::Int inSourceLevel,
    const GL::Int inDestinationLevel) const
{
  EXPECTS(ioDestinationTexture.GetFormat() == GetFormat());
  EXPECTS(inDestinationPosition + inSourceSize <= ioDestinationTexture.GetSize());

  auto source_position = One<Vec3i>();
  auto source_size = One<Vec3i>();
  auto destination_position = One<Vec3i>();
  for (int i = 0; i < N; ++i)
  {
    source_position[i] = inSourcePosition[i];
    source_size[i] = inSourceSize[i];
    destination_position[i] = inDestinationPosition[i];
  }

  GL::CopyImageSubData(GetGLId(),
      TTextureTarget,
      source_position[0],
      source_position[1],
      source_position[2],
      source_size[0],
      source_size[1],
      source_size[2],
      ioDestinationTexture.GetGLId(),
      TTextureTarget,
      destination_position[0],
      destination_position[1],
      destination_position[2],
      inSourceLevel,
      inDestinationLevel);
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::TexImageEmpty(const Veci<N>& inSize, const GL::ETextureFormat inFormat)
{
  auto texture_input_format = GL::ETextureInputFormat::RED;
  auto texture_input_data_type = GL::ETextureInputComponentFormat::FLOAT;
  if (GL::IsDepthOnlyFormat(inFormat) || GL::IsDepthStencilFormat(inFormat))
  {
    texture_input_format = GL::ETextureInputFormat::DEPTH_COMPONENT;
  }
  else if (GL::IsIntegerFormat(inFormat))
  {
    texture_input_format = GL::ETextureInputFormat::RED_INTEGER;
    texture_input_data_type = GL::ETextureInputComponentFormat::INT;
  }

  TexImage(inSize, texture_input_format, texture_input_data_type, Span<float>(nullptr, 0), inFormat, 0);
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::GenerateMipMap()
{
  GL::GenerateTextureMipMap(GetGLId());
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::SetWrapMode(const GL::EWrapMode inWrapMode)
{
  SetWrapModeR(inWrapMode);
  SetWrapModeS(inWrapMode);
  SetWrapModeT(inWrapMode);
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::SetWrapModeR(const GL::EWrapMode inWrapMode)
{
  GL::TextureParameteri(GetGLId(), GL::ETextureParameter::TEXTURE_WRAP_R, GL::EnumCast(inWrapMode));
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::SetWrapModeS(const GL::EWrapMode inWrapMode)
{
  GL::TextureParameteri(GetGLId(), GL::ETextureParameter::TEXTURE_WRAP_S, GL::EnumCast(inWrapMode));
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::SetWrapModeT(const GL::EWrapMode inWrapMode)
{
  GL::TextureParameteri(GetGLId(), GL::ETextureParameter::TEXTURE_WRAP_T, GL::EnumCast(inWrapMode));
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::SetMinFilterMode(const GL::EMinFilterMode inMinFilterMode)
{
  GL::TextureParameteri(GetGLId(), GL::ETextureParameter::TEXTURE_MIN_FILTER, GL::EnumCast(inMinFilterMode));
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::SetMagFilterMode(const GL::EMagFilterMode inMagFilterMode)
{
  GL::TextureParameteri(GetGLId(), GL::ETextureParameter::TEXTURE_MAG_FILTER, GL::EnumCast(inMagFilterMode));
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::Resize(const Veci<N>& inSize)
{
  EXPECTS(inSize >= One<Veci<N>>());

  if (inSize == mSize)
    return;

  TexImageEmpty(inSize, GetFormat());
}
}