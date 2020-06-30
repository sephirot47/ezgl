#include "ez/Texture3D.h"
#include "ez/GL.h"

namespace ez
{
Texture3D::Texture3D() : Texture()
{
  SetWrapMode(GL::EWrapMode::CLAMP_TO_EDGE);
  SetMinFilterMode(GL::EMinFilterMode::LINEAR);
  SetMagFilterMode(GL::EMagFilterMode::LINEAR);
}

Texture3D::Texture3D(const int inWidth, const int inHeight, const int inDepth, const GL::ETextureFormat inFormat)
    : Texture3D()
{
  SetEmptyData(inWidth, inHeight, inDepth, inFormat);
}

void Texture3D::Resize(const Vec3i& inSize) { Resize(inSize[0], inSize[1], inSize[2]); }
void Texture3D::Resize(const int inWidth, const int inHeight, const int inDepth)
{
  EXPECTS(inWidth >= 1);
  EXPECTS(inHeight >= 1);
  EXPECTS(inDepth >= 1);

  if (Vec3i(inWidth, inHeight, inDepth) == mSize)
    return;

  SetEmptyData(inWidth, inHeight, inDepth, GetFormat(), 0);
}

void Texture3D::SetEmptyData(const GL::Size inWidth,
    const GL::Size inHeight,
    const GL::Size inDepth,
    const GL::ETextureFormat inFormat,
    const GL::Int inMipMapLevel)
{
  SetData(inWidth,
      inHeight,
      inDepth,
      GL::ETextureInputFormat::RED,
      GL::ETextureInputComponentFormat::FLOAT,
      Span<float>(nullptr, 0),
      inFormat,
      inMipMapLevel);
}

GL::Size Texture3D::GetWidth() const { return mSize[0]; }
GL::Size Texture3D::GetHeight() const { return mSize[1]; }
GL::Size Texture3D::GetDepth() const { return mSize[2]; }
const Vec3i& Texture3D::GetSize() const { return mSize; }
}