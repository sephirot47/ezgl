#include "ez/GL.h"
#include "ez/GLGuard.h"
#include "ez/Texture2D.h"

namespace ez
{
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

}