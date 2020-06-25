#include "ez/GL.h"
#include "ez/GLGuard.h"
#include "ez/Texture3D.h"

namespace ez
{
template <typename T>
void Texture3D::SetData(const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::Size inDepth,
    const GL::ETextureInputFormat inInputFormat,
    const GL::ETextureInputComponentFormat inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureFormat inFormat,
    const GL::Int inMipMapLevel)
{
  const auto texture_bind_guard = BindGuarded();

  mSize = Vec3i(inWidth, inHeight, inDepth);
  mFormat = inFormat;

  GL::TexImage3D(GetTextureTarget(),
      inWidth,
      inHeight,
      inDepth,
      inInputFormat,
      inInputComponentFormat,
      inData,
      inFormat,
      inMipMapLevel);
}
}