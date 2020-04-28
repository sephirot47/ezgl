#pragma once

#include "GL.h"
#include <memory>

namespace egl
{
class Texture2D;

class RenderTarget
{
public:
  RenderTarget();
  RenderTarget(const GL::ETextureFormat inColorFormat, const GL::ETextureFormat inDepthFormat);
  RenderTarget(const RenderTarget&) = delete;
  RenderTarget& operator=(const RenderTarget&) = delete;
  RenderTarget(RenderTarget&& ioRHS) = default;
  RenderTarget& operator=(RenderTarget&& ioRHS) = delete;

  std::shared_ptr<Texture2D> GetColorTexture() { return mColorTexture; }
  std::shared_ptr<const Texture2D> GetColorTexture() const { return mColorTexture; }
  std::shared_ptr<Texture2D> GetDepthTexture() { return mDepthTexture; }
  std::shared_ptr<const Texture2D> GetDepthTexture() const { return mDepthTexture; }

  void Resize(const int inWidth, const int inHeight);
  void Resize(const Vec2i & inSize);

private:
  std::shared_ptr<Texture2D> mColorTexture;
  std::shared_ptr<Texture2D> mDepthTexture;
};

}