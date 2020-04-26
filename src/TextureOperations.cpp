#include "TextureOperations.h"
#include "MeshDrawData.h"
#include "MeshFactory.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "TextureFactory.h"

namespace egl
{

std::shared_ptr<ShaderProgram> TextureOperations::sDrawFullScreenTextureShaderProgram;
std::shared_ptr<MeshDrawData> TextureOperations::sPlaneDrawData;

void TextureOperations::DrawFullScreenQuad()
{
  TextureOperations::Init();

  const auto mesh_draw_data_bind_guard = sPlaneDrawData->BindGuarded();
  GL::DrawElements(GL::EPrimitivesType::TRIANGLES, sPlaneDrawData->GetNumberOfElements(), MeshDrawData::EBOGLIndexType);
}

void TextureOperations::DrawFullScreenTexture(const Texture2D& inTexture, const float inAlphaCutoff)
{
  DrawFullScreenTexture(inTexture, *TextureFactory::GetZeroTexture(), inAlphaCutoff);
}

void TextureOperations::DrawFullScreenTexture(const Texture2D& inTexture,
    const Texture2D& inDepthTexture,
    const float inAlphaCutoff)
{
  TextureOperations::Init();
  sDrawFullScreenTextureShaderProgram->Bind();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  inTexture.BindToTextureUnit(0);
  sDrawFullScreenTextureShaderProgram->SetUniformSafe("UTexture", 0);

  glTextureParameteri(inDepthTexture.GetGLId(), GL_TEXTURE_COMPARE_MODE, GL_NONE);
  inDepthTexture.BindToTextureUnit(1);
  sDrawFullScreenTextureShaderProgram->SetUniformSafe("UDepthTexture", 1);

  sDrawFullScreenTextureShaderProgram->SetUniformSafe("UAlphaCutoff", inAlphaCutoff);
  DrawFullScreenQuad();
}

void TextureOperations::Init()
{
  static bool sInited = false;
  if (!sInited)
  {
    sDrawFullScreenTextureShaderProgram
        = std::make_shared<ShaderProgram>(VertexShader(std::filesystem::path("../res/DrawFullScreenTexture.vert")),
            FragmentShader(std::filesystem::path("../res/DrawFullScreenTexture.frag")));

    auto plane_mesh = MeshFactory::GetPlane();
    plane_mesh.Transform(ScaleMat(All<Vec3f>(2.0f)));

    sPlaneDrawData = std::make_shared<MeshDrawData>(plane_mesh);

    sInited = true;
  }
}
}