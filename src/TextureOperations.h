#pragma once

#include "GL.h"
#include "TextureFactory.h"
#include <memory>

namespace egl
{
class ShaderProgram;
class Texture2D;
class MeshDrawData;

class TextureOperations
{
public:
  static void DrawFullScreenQuad();
  static void DrawFullScreenTexture(const Texture2D& inTexture);
  static void DrawFullScreenTexture(const Texture2D& inTexture, const Texture2D& inDepthTexture);

private:
  static void Init();
  static std::shared_ptr<ShaderProgram> sDrawFullScreenTextureShaderProgram;
  static std::shared_ptr<MeshDrawData> sPlaneDrawData;
};

}