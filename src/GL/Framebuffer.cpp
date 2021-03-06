#include <ez/Framebuffer.h>
#include <ez/GLGuard.h>

namespace ez
{
Framebuffer::Framebuffer() : Framebuffer(1, 1) {}
Framebuffer::Framebuffer(const Vec2i& inSize) : Framebuffer(inSize[0], inSize[1]) {}
Framebuffer::Framebuffer(const int inWidth, const int inHeight) { Resize(inWidth, inHeight); }
Framebuffer::~Framebuffer()
{
  if (mCreatedRenderbufferId != 0)
    GL::DeleteRenderbuffer(mCreatedRenderbufferId);
}

void Framebuffer::SetAttachment(const GL::EFramebufferAttachment inAttachment,
    const std::shared_ptr<Texture2D>& inTexture)
{
  const auto framebuffer_bind_guard = BindGuarded();

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
    GL::FramebufferTexture2D(inAttachment, GL::ETextureTarget::TEXTURE_2D, inTexture ? inTexture->GetGLId() : 0);
  }
}

void Framebuffer::CreateRenderbuffer(const GL::EFramebufferAttachment inRenderbufferAttachment,
    const GL::ETextureFormat inFormat)
{
  if (mRenderbufferTexture != nullptr)
    THROW_EXCEPTION("Can't create a Renderbuffer because a texture has been attached before as Renderbuffer.");

  if (mCreatedRenderbufferId != GL::InvalidId)
    THROW_EXCEPTION("Can't create two Renderbuffers for the same Framebuffer.");

  if (IsColorAttachment(inRenderbufferAttachment))
    THROW_EXCEPTION(
        "Incorrect attachment for Renderbuffer. It must not be a color attachment, but a depth/stencil one.");

  const auto framebuffer_bind_guard = BindGuarded();

  mCreatedRenderbufferId = GL::CreateRenderbuffer();
  mCreatedRenderbufferFormat = inFormat;

  const GLBindGuard<GL::EBindingType::RENDERBUFFER> renderbuffer_bind_guard;
  GL::BindRenderbuffer(mCreatedRenderbufferId);

  GL::RenderbufferStorage(inFormat, mSize[0], mSize[1]);
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
      texture->Resize(mSize);
  }

  if (mCreatedRenderbufferId != GL::InvalidId)
  {
    const GLBindGuard<GL::EBindingType::RENDERBUFFER> renderbuffer_bind_guard;
    GL::BindRenderbuffer(mCreatedRenderbufferId);

    GL::RenderbufferStorage(mCreatedRenderbufferFormat, mSize[0], mSize[1]);
  }
  else if (mRenderbufferTexture)
  {
    mRenderbufferTexture->Resize(Vec2i { inWidth, inHeight });
  }
}

void Framebuffer::CheckFramebufferIsComplete() const
{
  const auto framebuffer_bind_guard = BindGuarded();

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