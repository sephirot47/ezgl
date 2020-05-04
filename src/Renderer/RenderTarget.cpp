#include "ezgl/RenderTarget.h"
#include "ezgl/Framebuffer.h"
#include "ezgl/Texture2D.h"

namespace egl
{
RenderTarget::RenderTarget() : RenderTarget(GL::ETextureFormat::RGBA8, GL::ETextureFormat::DEPTH24_STENCIL8) {}

RenderTarget::RenderTarget(const GL::ETextureFormat inColorFormat, const GL::ETextureFormat inDepthFormat)
{
  EXPECTS(GL::IsColorFormat(inColorFormat));
  EXPECTS(GL::IsDepthOnlyFormat(inDepthFormat) || GL::IsDepthStencilFormat(inDepthFormat));

  mColorTexture = std::make_shared<Texture2D>(1, 1, inColorFormat);
  mDepthTexture = std::make_shared<Texture2D>(1, 1, inDepthFormat);

  mFramebuffer = std::make_shared<Framebuffer>(1, 1);
  mFramebuffer->SetAttachment(GL::EFramebufferAttachment::COLOR_ATTACHMENT0, mColorTexture);
  if (GL::IsDepthOnlyFormat(inDepthFormat))
  {
    mFramebuffer->SetAttachment(GL::EFramebufferAttachment::DEPTH_ATTACHMENT, mDepthTexture);
  }
  else
  {
    mFramebuffer->SetAttachment(GL::EFramebufferAttachment::DEPTH_STENCIL_ATTACHMENT, mDepthTexture);
  }
  mFramebuffer->CheckFramebufferIsComplete();
}

void RenderTarget::Clear(const Color4f& inClearColor, const float inClearDepth)
{
  const GLGuardType render_target_guard;

  Bind();
  GL::ClearColor(inClearColor);
  GL::ClearDepth(inClearDepth);
}

void RenderTarget::Bind() { mFramebuffer->Bind(); }

void RenderTarget::UnBind() { mFramebuffer->UnBind(); }

void RenderTarget::Resize(const int inWidth, const int inHeight) { Resize(Vec2i { inWidth, inHeight }); }

void RenderTarget::Resize(const Vec2i& inSize)
{
  EXPECTS(inSize[0] >= 1);
  EXPECTS(inSize[1] >= 1);
  mFramebuffer->Resize(inSize);
}
}