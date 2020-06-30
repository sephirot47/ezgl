#include "ez/Texture.h"

namespace ez
{

template <GL::ETextureTarget TTextureTarget>
Texture<TTextureTarget>::Texture()
{
  // For texture completeness
  SetMinFilterMode(GL::EMinFilterMode::NEAREST);
  SetMagFilterMode(GL::EMagFilterMode::NEAREST);
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
    const GL::Int inLayer)
{
  GL::BindImageTexture(inImageUnit, GetGLId(), GetFormat(), inAccess, inLevel, inLayered, inLayer);
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

}