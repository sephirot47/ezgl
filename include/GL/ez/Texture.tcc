#include "ez/Texture.h"

namespace ez
{

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::BindToTextureUnit(const GL::Size& inTextureUnit) const
{
  EXPECTS((inTextureUnit + GL_TEXTURE0) <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  GL::BindTextureUnit(inTextureUnit, GetGLId());
}

template <GL::ETextureTarget TTextureTarget>
void Texture<TTextureTarget>::GenerateMipMap()
{
  GL::GenerateTextureMipMap(GetGLId());
}
}