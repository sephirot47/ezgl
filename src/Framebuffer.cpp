#include "Framebuffer.h"

namespace egl
{
Framebuffer::Framebuffer() : Framebuffer(1, 1) {}
Framebuffer::Framebuffer(const Vec2i& inSize) : Framebuffer(inSize[0], inSize[1]) {}
Framebuffer::Framebuffer(const int inWidth, const int inHeight) { Resize(inWidth, inHeight); }

void Framebuffer::SetAttachment(const GL::EFramebufferAttachment inAttachment,
    const std::shared_ptr<Texture2D>& inTexture)
{
  GL_BIND_GUARD(GL::EBindingType::FRAMEBUFFER);
  Bind();

  if (IsColorAttachment(inAttachment))
  {
    const auto color_attachment_index = GetColorAttachmentIndex(inAttachment);
    mColorTextures.at(color_attachment_index) = inTexture;
    GL::FramebufferTexture2D(inAttachment, GL::ETextureTarget::TEXTURE_2D, inTexture ? inTexture->GetGLId() : 0);
  }
  else
  {
    if (mCreatedRenderbufferId != GL::InvalidId)
      THROW_EXCEPTION("Can't attach Renderbuffer because one has been explicitly created before.");

    mRenderbufferTexture = inTexture;
    mRenderbufferAttachment = inAttachment;
    GL::FramebufferRenderbuffer(inAttachment, inTexture ? inTexture->GetGLId() : 0);
  }
}

void Framebuffer::CreateRenderbuffer(const GL::EFramebufferAttachment inRenderbufferAttachment,
    const GL::ETextureInternalFormat inInternalFormat)
{
  if (mRenderbufferTexture != nullptr)
    THROW_EXCEPTION("Can't create a Renderbuffer because a texture has been attached before as Renderbuffer.");

  if (mCreatedRenderbufferId != GL::InvalidId)
    THROW_EXCEPTION("Can't create two Renderbuffers for the same Framebuffer.");

  if (IsColorAttachment(inRenderbufferAttachment))
    THROW_EXCEPTION(
        "Incorrect attachment for Renderbuffer. It must not be a color attachment, but a depth/stencil one.");

  GL_BIND_GUARD(GL::EBindingType::FRAMEBUFFER);
  Bind();

  mCreatedRenderbufferId = GL::CreateRenderbuffer();
  mCreatedRenderbufferInternalFormat = inInternalFormat;

  GL_BIND_GUARD(GL::EBindingType::RENDERBUFFER);
  GL::BindRenderbuffer(mCreatedRenderbufferId);

  GL::RenderbufferStorage(inInternalFormat, mSize[0], mSize[1]);
  GL::FramebufferRenderbuffer(inRenderbufferAttachment, mCreatedRenderbufferId);
}

void Framebuffer::Resize(const Vec2i& inSize) { Resize(inSize[0], inSize[1]); }
void Framebuffer::Resize(const int inWidth, const int inHeight)
{
  EXPECTS(inWidth >= 1);
  EXPECTS(inHeight >= 1);

  if (Vec2i(inWidth, inHeight) == mSize)
    return;

  mSize = Vec2i(inWidth, inHeight);
  for (auto& texture : mColorTextures)
  {
    if (texture)
      texture->Resize(inWidth, inHeight);
  }

  if (mCreatedRenderbufferId != GL::InvalidId)
  {
    GL_BIND_GUARD(GL::EBindingType::RENDERBUFFER);
    GL::BindRenderbuffer(mCreatedRenderbufferId);

    GL::RenderbufferStorage(mCreatedRenderbufferInternalFormat, mSize[0], mSize[1]);
  }
  else if (mRenderbufferTexture)
  {
    mRenderbufferTexture->Resize(inWidth, inHeight);
  }
}

void Framebuffer::CheckFramebufferIsComplete() const
{
  GL_BIND_GUARD(GL::EBindingType::FRAMEBUFFER);
  Bind();

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    THROW_EXCEPTION("Framebuffer is not complete.");
}

std::shared_ptr<Texture2D> Framebuffer::GetAttachment(const GL::EFramebufferAttachment inAttachment) const
{
  if (IsColorAttachment(inAttachment))
  {
    const auto color_attachment_index = GetColorAttachmentIndex(inAttachment);
    return mColorTextures.at(color_attachment_index);
  }
  else if (mRenderbufferAttachment == inAttachment)
  {
    return mRenderbufferTexture;
  }
  return nullptr;
}

std::optional<GL::Id> Framebuffer::GetCreatedRenderbufferId() const
{
  return mCreatedRenderbufferId != GL::InvalidId ? std::make_optional(mCreatedRenderbufferId) : std::nullopt;
}

const Vec2i& Framebuffer::GetSize() const { return mSize; }

std::size_t Framebuffer::GetColorAttachmentIndex(const GL::EFramebufferAttachment inColorAttachment)
{
  EXPECTS(IsColorAttachment(inColorAttachment));
  return static_cast<std::size_t>(static_cast<std::size_t>(inColorAttachment) - GL_COLOR_ATTACHMENT0);
}

bool Framebuffer::IsColorAttachment(const GL::EFramebufferAttachment inAttachment)
{
  return (inAttachment >= GL::EFramebufferAttachment::COLOR_ATTACHMENT0
      && inAttachment <= GL::EFramebufferAttachment::COLOR_ATTACHMENT15);
}
}