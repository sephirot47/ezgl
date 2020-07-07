#include "ez/GL.h"
#include "ez/GLGuard.h"
#include "ez/Texture2D.h"

namespace ez
{
template <typename TImageValueType>
Texture2D::Texture2D(const Image2D<TImageValueType>& inImage2D) : Texture2D()
{
  LoadImage(inImage2D);
}

template <typename TImageValueType>
void Texture2D::LoadImage(const Image2D<TImageValueType>& inImage2D,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  constexpr auto input_format = GLTypeTraits<TImageValueType>::GLTextureInputFormat;
  constexpr auto input_component_format = GLTypeTraits<TImageValueType>::GLTextureComponentFormat;
  TexImage<TImageValueType>(Vec2i { inImage2D.GetWidth(), inImage2D.GetHeight() },
      input_format,
      input_component_format,
      inImage2D.GetData(),
      inFormat,
      inMipMapLevel);
}
}