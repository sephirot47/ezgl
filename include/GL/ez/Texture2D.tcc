#include <ez/GL.h>
#include <ez/GLGuard.h>
#include "ez/Texture2D.h"

namespace ez
{
template <typename TImageValueType>
Texture2D::Texture2D(const Image2D<TImageValueType>& inImage2D) : Texture2D()
{
  TexImageEmpty(Vec2i { inImage2D.GetWidth(), inImage2D.GetHeight() }, GLTypeTraits<TImageValueType>::GLTextureFormat);
  LoadImage(inImage2D);
}

template <typename TImageValueType>
void Texture2D::LoadImage(const Image2D<TImageValueType>& inImage2D, const GL::Int& inMipMapLevel)
{
  constexpr auto input_format = GLTypeTraits<TImageValueType>::GLTextureInputFormat;
  constexpr auto input_data_type = GLTypeTraits<TImageValueType>::GLComponentType;
  TextureSubImage<TImageValueType>(Vec2i { 0, 0 },
      Vec2i { inImage2D.GetWidth(), inImage2D.GetHeight() },
      input_format,
      input_data_type,
      inImage2D.GetData(),
      inMipMapLevel);
}
}