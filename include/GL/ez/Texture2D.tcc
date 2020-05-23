#include "ez/GL.h"
#include "ez/GLGuard.h"
#include "ez/Texture2D.h"

namespace ez
{
template <typename TImageValueType>
Texture2D::Texture2D(const Image2D<TImageValueType>& inImage2D) : Texture2D()
{
  SetData(inImage2D);
}

template <typename T>
void Texture2D::SetData(const GL::Size& inWidth,
    const GL::Size& inHeight,
    const GL::ETextureInputFormat& inInputFormat,
    const GL::ETextureInputComponentFormat& inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  const auto texture_bind_guard = BindGuarded();

  mSize = Vec2i(inWidth, inHeight);
  mFormat = inFormat;

  GL::TexImage2D(GetTextureTarget(),
      inWidth,
      inHeight,
      inInputFormat,
      inInputComponentFormat,
      inData,
      inFormat,
      inMipMapLevel);
}

template <typename TImageValueType>
void Texture2D::SetData(const Image2D<TImageValueType>& inImage2D,
    const GL::ETextureFormat& inFormat,
    const GL::Int& inMipMapLevel)
{
  constexpr auto input_format = GLTypeTraits<TImageValueType>::GLTextureInputFormat;
  constexpr auto input_component_format = GLTypeTraits<TImageValueType>::GLTextureComponentFormat;
  SetData<TImageValueType>(inImage2D.GetWidth(),
      inImage2D.GetHeight(),
      input_format,
      input_component_format,
      inImage2D.GetData(),
      inFormat,
      inMipMapLevel);
}
}