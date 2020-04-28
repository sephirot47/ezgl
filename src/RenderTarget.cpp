#include "RenderTarget.h"
#include "Texture2D.h"

namespace egl
{
RenderTarget::RenderTarget()
    : RenderTarget(GL::ETextureInternalFormat::RGBA8, GL::ETextureInternalFormat::DEPTH24_STENCIL8)
{
}

RenderTarget::RenderTarget(const GL::ETextureInternalFormat inColorFormat,
    const GL::ETextureInternalFormat inDepthFormat)
{
  EXPECTS(GL::IsColorFormat(inColorFormat));
  EXPECTS(GL::IsDepthOnlyFormat(inDepthFormat) || GL::IsDepthStencilFormat(inDepthFormat));

  mColorTexture = std::make_shared<Texture2D>(1, 1, inColorFormat);
  mDepthTexture = std::make_shared<Texture2D>(1, 1, inDepthFormat);
}

void RenderTarget::Resize(const int inWidth, const int inHeight) { Resize(Vec2i { inWidth, inHeight }); }

void RenderTarget::Resize(const Vec2i& inSize)
{
  EXPECTS(inSize[0] >= 1);
  EXPECTS(inSize[1] >= 1);
  mColorTexture->Resize(inSize);
  mDepthTexture->Resize(inSize);
}
}