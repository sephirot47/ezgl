#pragma once

#include "GLBindableObject.h"
#include "Math.h"
#include "Texture2D.h"
#include <array>
#include <memory>
#include <variant>
#include <vector>

namespace egl
{
class Framebuffer : public GLBindableObject<GL::EBindingType::FRAMEBUFFER>
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::FRAMEBUFFER>;

  Framebuffer();
  explicit Framebuffer(const Vec2i& inSize);
  Framebuffer(const int inWidth, const int inHeight);
  Framebuffer(Framebuffer&& ioRHS);
  ~Framebuffer() override;

  using GLBindableObject<GL::EBindingType::FRAMEBUFFER>::GetGLId;

  void SetAttachment(const GL::EFramebufferAttachment inAttachment, const std::shared_ptr<Texture2D>& inTexture);
  void CreateRenderbuffer(const GL::EFramebufferAttachment inRenderbufferAttachment,
      const GL::ETextureInternalFormat inInternalFormat);
  void SetRenderbuffer(const GL::EFramebufferAttachment inRenderbufferAttachment,
      const std::shared_ptr<Texture2D>& inTexture);
  void Resize(const Vec2i& inSize);
  void Resize(const int inWidth, const int inHeight);

  void CheckFramebufferIsComplete() const;
  std::shared_ptr<Texture2D> GetAttachment(const GL::EFramebufferAttachment inAttachment) const;
  std::optional<GL::Id> GetCreatedRenderbufferId() const;

  const Vec2i& GetSize() const;

private:
  Vec2i mSize = Zero<Vec2i>();

  static constexpr auto MaxNumColorAttachments = (GL_COLOR_ATTACHMENT15 - GL_COLOR_ATTACHMENT0 + 1);
  std::array<std::shared_ptr<Texture2D>, MaxNumColorAttachments> mColorTextures;

  // TODO: Use variant for Renderbuffer
  GL::Id mCreatedRenderbufferId = GL::InvalidId;
  GL::ETextureInternalFormat mCreatedRenderbufferInternalFormat = GL::ETextureInternalFormat::RED;

  std::shared_ptr<Texture2D> mRenderbufferTexture;
  GL::EFramebufferAttachment mRenderbufferAttachment = GL::EFramebufferAttachment::DEPTH_STENCIL_ATTACHMENT;

  static std::size_t GetColorAttachmentIndex(const GL::EFramebufferAttachment inColorAttachment);
  static bool IsColorAttachment(const GL::EFramebufferAttachment inAttachment);
};
}