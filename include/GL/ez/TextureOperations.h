#pragma once

#include <ez/GL.h>
#include <ez/TextureFactory.h>
#include <memory>

namespace ez
{
class ShaderProgram;
class Texture2D;
class MeshDrawData;

class TextureOperations final
{
public:
  static void DrawFullScreenQuad();
  static void DrawFullScreenTexture(const Texture2D& inTexture);
  static void DrawFullScreenTexture(const Texture2D& inTexture, const Texture2D& inDepthTexture);

private:
  static void Init();
  static std::shared_ptr<MeshDrawData> sPlaneDrawData;
};

}