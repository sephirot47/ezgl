#include "Texture.h"

namespace egl
{

template <GL::ETextureTarget TTextureTarget, GL::EBindingType TBindingType>
void Texture<TTextureTarget, TBindingType>::BindToTextureUnit(const GL::Size& inTextureUnit) const
{
  EXPECTS((inTextureUnit + GL_TEXTURE0) <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  GL::BindTextureUnit(inTextureUnit, GetGLId());
}

template <GL::ETextureTarget TTextureTarget, GL::EBindingType TBindingType>
void Texture<TTextureTarget, TBindingType>::GenerateMipMap()
{
  GL::GenerateTextureMipMap(GetGLId());
}
}