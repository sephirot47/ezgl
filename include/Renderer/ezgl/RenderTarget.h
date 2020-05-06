#pragma once

#include "ezgl/GL.h"
#include "ezgl/GLGuard.h"
#include "ezgl/MathInitializers.h"
#include <memory>

namespace ez
{
class Texture2D;
class Framebuffer;

class RenderTarget final
{
public:
  using GLGuardType = GLBindGuard<GL::EBindingType::FRAMEBUFFER>;

  RenderTarget();
  RenderTarget(const GL::ETextureFormat inColorFormat, const GL::ETextureFormat inDepthFormat);
  RenderTarget(const RenderTarget&) = delete;
  RenderTarget& operator=(const RenderTarget&) = delete;
  RenderTarget(RenderTarget&& ioRHS) = default;
  RenderTarget& operator=(RenderTarget&& ioRHS) = delete;

  void Clear(const Color4f& inClearColor = Black<Color4f>(), const float inClearDepth = 1.0f);
  void Bind();
  void UnBind();

  std::shared_ptr<Texture2D> GetColorTexture() { return mColorTexture; }
  std::shared_ptr<const Texture2D> GetColorTexture() const { return mColorTexture; }
  std::shared_ptr<Texture2D> GetDepthTexture() { return mDepthTexture; }
  std::shared_ptr<const Texture2D> GetDepthTexture() const { return mDepthTexture; }

  void Resize(const int inWidth, const int inHeight);
  void Resize(const Vec2i& inSize);

private:
  std::shared_ptr<Texture2D> mColorTexture;
  std::shared_ptr<Texture2D> mDepthTexture;
  std::shared_ptr<Framebuffer> mFramebuffer;
};

}