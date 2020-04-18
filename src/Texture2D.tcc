#include "GL.h"
#include "Texture2D.h"

namespace egl
{
template <typename T>
void Texture2D::SetData(const GL::Size& inWidth,
    const GL::Size& inHeight,
    const GL::ETextureInputFormat& inInputFormat,
    const GL::ETextureInputComponentFormat& inInputComponentFormat,
    const Span<T>& inData,
    const GL::ETextureInternalFormat& inInternalFormat,
    const GL::Int& inMipMapLevel)
{
  Bind();
  GL::TexImage2D(GetTextureTarget(), inWidth, inHeight, inInputFormat, inInputComponentFormat, inData, inInternalFormat, inMipMapLevel);
}

}