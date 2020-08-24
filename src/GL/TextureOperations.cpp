#include <ez/TextureOperations.h>
#include <ez/MeshDrawData.h>
#include <ez/MeshFactory.h>
#include <ez/ShaderProgram.h>
#include <ez/ShaderProgramFactory.h>
#include "ez/Texture2D.h"
#include <ez/TextureFactory.h>

namespace ez
{

std::shared_ptr<MeshDrawData> TextureOperations::sPlaneDrawData;

void TextureOperations::DrawFullScreenQuad()
{
  TextureOperations::Init();

  const auto mesh_draw_data_bind_guard = sPlaneDrawData->BindGuarded();
  GL::DrawElements(GL::EPrimitivesType::TRIANGLES, sPlaneDrawData->GetNumberOfElements(), MeshDrawData::EBOGLIndexType);
}

void TextureOperations::DrawFullScreenTexture(const Texture2D& inTexture)
{
  DrawFullScreenTexture(inTexture, *TextureFactory::GetZeroTexture());
}

void TextureOperations::DrawFullScreenTexture(const Texture2D& inTexture, const Texture2D& inDepthTexture)
{
  TextureOperations::Init();

  const auto draw_full_screen_texture_shader_program = ShaderProgramFactory::GetDrawFullScreenTextureShaderProgram();
  draw_full_screen_texture_shader_program->Bind();

  inTexture.BindToTextureUnit(0);
  draw_full_screen_texture_shader_program->SetUniformSafe("UTexture", 0);

  const GLDepthMaskGuard depth_mask_guard;
  GL::DepthMask(true);

  const GLDepthFuncGuard depth_func_guard;
  GL::DepthFunc(GL::EDepthFunc::LEQUAL);

  const GLTextureParameteriGuard<GL::ETextureParameter::TEXTURE_COMPARE_MODE> texture_compare_mode_guard(
      inDepthTexture.GetGLId());
  GL::SetTextureCompareMode(inDepthTexture.GetGLId(), GL::ETextureCompareMode::NONE);

  inDepthTexture.BindToTextureUnit(1);
  draw_full_screen_texture_shader_program->SetUniformSafe("UDepthTexture", 1);

  DrawFullScreenQuad();
}

void TextureOperations::Init()
{
  static bool sInited = false;
  if (!sInited)
  {
    auto plane_mesh = MeshFactory::GetPlane();
    plane_mesh.Transform(ScaleMat(All<Vec3f>(2.0f)));

    sPlaneDrawData = std::make_shared<MeshDrawData>(plane_mesh);

    sInited = true;
  }
}
}