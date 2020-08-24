#include <ez/Camera.h>
#include <ez/CameraControllerFly.h>
#include <ez/EBO.h>
#include <ez/FileUtils.h>
#include <ez/Framebuffer.h>
#include "ez/Image2D.h"
#include <ez/Macros.h>
#include "ez/Material3D.h"
#include <ez/Math.h>
#include <ez/Mesh.h>
#include <ez/MeshDrawData.h>
#include <ez/MeshFactory.h>
#include <ez/RenderTarget.h>
#include <ez/Renderer.h>
#include "ez/Renderer2D.h"
#include "ez/Renderer3D.h"
#include <ez/Segment.h>
#include <ez/Shader.h>
#include <ez/ShaderProgram.h>
#include <ez/ShaderProgramFactory.h>
#include <ez/Span.h>
#include <ez/StreamOperators.h>
#include <ez/Texture.h>
#include "ez/Texture2D.h"
#include <ez/TextureFactory.h>
#include <ez/TextureOperations.h>
#include <ez/VAO.h>
#include <ez/VAOVertexAttrib.h>
#include <ez/VBO.h>
#include <ez/VariadicRepeat.h>
#include <ez/Window.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace ez;

int main(int argc, const char** argv)
{
  const auto create_test_references_option = "--create-test-references";
  if (argc >= 2 && std::string(argv[1]) != create_test_references_option)
  {
    std::cerr << "Unknown option '" << argv[1] << "'. Did you mean '" << create_test_references_option << "'?"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  const auto create_test_references = (argc >= 2 && std::string(argv[1]) == create_test_references_option);

  srand(time(0));

  Window::CreateOptions create_options;
  create_options.mWidth = 800;
  create_options.mHeight = 800;
  create_options.mUseAntialiasing = false;
  const auto window = std::make_shared<Window>(create_options);

  const auto test_mesh = MeshFactory::GetTorus(28, 29, 0.5f);
  const auto sphere = MeshFactory::GetSphere(35, 42);
  const auto hemisphere = MeshFactory::GetHemisphere(22, 15);
  const auto torus = MeshFactory::GetTorus(25, 36, 0.8f);
  const auto box = MeshFactory::GetBox();
  const auto plane = MeshFactory::GetPlane(5, 9);
  const auto cone = MeshFactory::GetCone(35);

  const auto render_target = std::make_shared<RenderTarget>();

  const auto camera3d = std::make_shared<PerspectiveCameraf>();
  camera3d->SetPosition(Vec3f(3.0f, 4.0f, -2.0f));
  camera3d->LookAtPoint(Vec3f(0.5f, 0.2f, -0.5f));

  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera3d);
  camera_controller_fly.SetWindow(window);

  auto time = Seconds { 0 };
  Renderer2D renderer2D;
  Renderer3D renderer3D;
  bool tests_succeeded = true;
  window->Loop([&](const DeltaTime& inDeltaTime) {
    time += inDeltaTime;

    GL::ClearColor(Gray<Color4f>());
    GL::ClearDepth();

    renderer3D.ResetState();
    renderer3D.SetCamera(camera3d);
    renderer3D.AdaptToWindow(*window);
    renderer3D.Clear(Color4f(0.4f, 0.4f, 1.0f, 1.0f), 1.0f);
    render_target->Resize(window->GetFramebufferSize());
    render_target->Clear(White<Color4f>());

    renderer2D.ResetState();
    renderer2D.AdaptToWindow(*window);
    renderer2D.Clear(Color4f(0.1f, 0.8f, 0.2f, 1.0f));

    // Light
    renderer3D.AddDirectionalLight(Normalized(Vec3f(1.0f, -1.0f, 0.0f)), Purple<Color3f>());
    renderer3D.SetSceneAmbientColor(WithValue(Blue<Color3f>(), 0.3f));

    // Ground plane
    {
      const auto point_lights_guard = renderer3D.GetGuard<Renderer3D::EStateId::POINT_LIGHTS>();
      renderer3D.AddPointLight(Up<Vec3f>() * 5.0f, 20.0f, Green<Color3f>());

      const auto model_matrix_guard = renderer3D.GetGuard<Renderer3D::EStateId::TRANSFORM_MATRIX>();
      renderer3D.Translate(Up<Vec3f>() * 0.3f);
      renderer3D.Rotate(AngleAxis(QuarterCircleRads(), Right<Vec3f>()));
      renderer3D.Scale(XY1(All<Vec3f>(200.0f)));

      const auto material_guard = renderer3D.GetGuard<Renderer3D::EStateId::MATERIAL>();

      const auto cull_face_enabled_guard = renderer3D.GetGuard<Renderer3D::EStateId::CULL_FACE_ENABLED>();
      renderer3D.SetCullFaceEnabled(false);

      renderer3D.DrawMesh(plane);
    }

    // Some 2D
    {
      const auto points
          = std::array { Vec2f(30.0f, 40.0f), Vec2f(500.0f, 400.0f), Vec2f(400.0f, 300.0f), Vec2f(100.0f, 600.0f) };

      renderer2D.SetPointSize(10.0f);
      renderer2D.GetMaterial().SetColor(Red<Color4f>());
      renderer2D.DrawPoints(MakeSpan(points));

      renderer2D.SetPointSize(6.0f);
      renderer2D.GetMaterial().SetColor(Green<Color4f>());
      renderer2D.DrawPoints(MakeSpan(points));

      renderer2D.Blit(*render_target);
      renderer2D.Blit();
    }

    // Some objects
    {
      const auto material_guard = renderer3D.GetGuard<Renderer3D::EStateId::MATERIAL>();
      renderer3D.GetMaterial().SetSpecularExponent(80.0f);
      renderer3D.GetMaterial().SetLightingEnabled(true);
      renderer3D.GetMaterial().SetSpecularIntensity(2.0f);

      const auto model_matrix_guard = renderer3D.GetGuard<Renderer3D::EStateId::TRANSFORM_MATRIX>();

      renderer3D.SetBlendEnabled(true);
      {
        const auto blend_factors_guard = renderer3D.GetGuard<RendererGPU::EStateId::BLEND_FACTORS>();
        const auto blend_enabled_guard = renderer3D.GetGuard<RendererGPU::EStateId::BLEND_ENABLED>();
        renderer3D.SetBlendFuncRGB(GL::EBlendFactor::SRC_ALPHA, GL::EBlendFactor::ONE_MINUS_SRC_ALPHA);

        renderer3D.GetMaterial().SetDiffuseColor(WithAlpha(Cyan<Color4f>(), 0.8f));
        renderer3D.Translate(Up<Vec3f>() * 1.5f);
        renderer3D.DrawMesh(sphere);

        renderer3D.Translate(Right<Vec3f>() * 1.5f);
        renderer3D.Rotate(AngleAxis(0.47f, NormalizedSafe(Vec3f(1.0f, -1.0f, -1.0f))));
        renderer3D.Scale(0.5f);
        renderer3D.GetMaterial().SetDiffuseColor(WithAlpha(Red<Color4f>(), 0.3f));
        renderer3D.DrawMesh(box);

        renderer3D.SetBlendEnabled(false);
      }

      renderer3D.Translate(Back<Vec3f>() * 2.5f);
      renderer3D.Rotate(AngleAxis(1.337f, NormalizedSafe(Vec3f(1.0f, -1.0f, -1.0f))));
      renderer3D.Scale(2.5f);
      renderer3D.GetMaterial().SetDiffuseColor(WithAlpha(Yellow<Color4f>(), 0.5f));
      renderer3D.DrawMesh(torus);

      renderer3D.ResetMaterial();
      renderer3D.ResetTransformMatrix();
      renderer3D.AddDirectionalLight(Normalized(Vec3f(-1.0f, -1.0f, 0.0f)), White<Color3f>() * 1.3f);
      renderer3D.Translate(Right<Vec3f>() * 2.5f + Up<Vec3f>() * -0.5f);
      renderer3D.Rotate(AngleAxis(QuarterCircleRads(), Right<Vec3f>()));
      renderer3D.Scale(Vec3f(0.7f, 0.5f, 1.5f));
      renderer3D.GetMaterial().SetDiffuseColor(WithAlpha(Red<Color4f>(), 0.75f));
      renderer3D.DrawMesh(cone);

      renderer3D.ResetMaterial();
      renderer3D.ResetTransformMatrix();
      renderer3D.SetLineWidth(5.0f);
      renderer3D.DrawLineStrip(MakeSpan({ Vec3f(0.0f, 0.0f, 0.0f), Vec3f(3.0f, 1.0f, 2.0f), Vec3f(1.0f, 5.0f, 0.0f) }));

      {
        const auto cull_face_guard = renderer3D.GetGuard<Renderer3D::EStateId::CULL_FACE_ENABLED>();
        renderer3D.SetCullFaceEnabled(false);

        renderer3D.Scale(1.0f);
        const auto triangles
            = std::array { Triangle3f(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(2.0f, 2.0f, 2.0f), Vec3f(-3.0f, -4.0f, -2.0f)),
                Triangle3f(Vec3f(-4.0f, 5.0f, -3.0f), Vec3f(2.0f, 0.0f, 2.0f), Vec3f(-1.0f, 0.0f, 0.0f)) };
        renderer3D.DrawTriangles(MakeSpan(triangles));

        renderer3D.Translate(Up<Vec3f>() * 3.0f);
        renderer3D.Scale(1.0f);
        renderer3D.GetMaterial().SetDiffuseColor(Red<Color4f>());
        renderer3D.DrawCircle(10);
        renderer3D.GetMaterial().SetDiffuseColor(Green<Color4f>());
        renderer3D.DrawCircleBoundary(10);

        renderer3D.Scale(2.0f);
        renderer3D.GetMaterial().SetDiffuseColor(Red<Color4f>());
        renderer3D.DrawCircleSection(HalfCircleRads(), 10);
        renderer3D.GetMaterial().SetDiffuseColor(Green<Color4f>());
        renderer3D.DrawCircleSectionBoundary(HalfCircleRads(), 10);
      }
    }

    renderer3D.Blit(*render_target);
    renderer3D.Blit();

    // Some more 2D
    {
      renderer2D.Clear(Zero<Color4f>());

      const auto points = std::array { Vec2f { 300.0f, 100.0f }, Vec2f { 500.0f, 550.0f }, Vec2f { 200.0f, 350.0f } };
      const auto segments = std::array { Segment2f { points[0], points[1] },
        Segment2f { points[1], points[2] },
        Segment2f { points[2], points[0] } };

      renderer2D.SetLineWidth(5.0f);
      renderer2D.GetMaterial().SetColor(Yellow<Color4f>());
      renderer2D.DrawSegments(MakeSpan(segments));
      renderer2D.Scale(0.8f);
      renderer2D.GetMaterial().SetColor(Red<Color4f>());
      renderer2D.DrawTriangle(Triangle2f { points[0], points[1], points[2] });
      renderer2D.GetMaterial().SetColor(Cyan<Color4f>());
      renderer2D.DrawTriangleBoundary(Triangle2f { points[0], points[1], points[2] });

      renderer2D.SetLineWidth(1.0f);
      renderer2D.Translate(Vec2f(200.0f, 200.0f));
      renderer2D.Scale(Vec2f(0.1f, 0.05f));

      for (int i = 0; i < 5; ++i)
      {
        for (int j = 0; j < 30; ++j)
        {
          renderer2D.GetMaterial().SetColor(
              HSVToRGB(RGBToHSV(HSVToRGB(Color4f(((i / 5.0f) + j) / 30.0f, 1.0f, 1.0f, 1.0f)))));
          renderer2D.Rotate(FullCircleRads() / 30.0);
          renderer2D.DrawSegments(MakeSpan(segments));
        }
        renderer2D.Scale(1.5f);
      }

      renderer2D.DrawLineStrip(MakeSpan({ Vec2f(0.0f, 10.0f), Vec2f(10.0f, 30.0f) }));

      renderer2D.SetLineWidth(10.0f);

      renderer2D.ResetTransformMatrix();
      renderer2D.Translate(Vec2f(500.0f, 500.0f));
      renderer2D.Scale(80.0f);
      renderer2D.DrawCircleSection(FullCircleRads() * 0.3f, 20);
      renderer2D.PushMaterial();
      renderer2D.GetMaterial().SetColor(Black<Color4f>());
      renderer2D.DrawCircleSectionBoundary(FullCircleRads() * 0.3f, 5);
      renderer2D.PopMaterial();

      renderer2D.ResetTransformMatrix();
      renderer2D.Translate(Vec2f(500.0f, 500.0f));
      renderer2D.Scale(40.0f);
      renderer2D.DrawCircle(10);
      renderer2D.PushMaterial();
      renderer2D.GetMaterial().SetColor(Black<Color4f>());
      renderer2D.DrawCircleBoundary(10);
      renderer2D.PopMaterial();

      renderer2D.Blit(*render_target);
      renderer2D.Blit();
    }

    camera_controller_fly.Update(inDeltaTime);

    // Check test result
    {
      const std::filesystem::path reference_image_name = "test-reference.png";
      if (!create_test_references && !std::filesystem::exists(reference_image_name))
      {
        std::cerr << "Test reference images have not been created (" << reference_image_name
                  << " does not exist). Please execute this program with " << create_test_references_option
                  << " parameter." << std::endl;
        tests_succeeded = false;
        return Window::ELoopResult::END_LOOP;
      }

      const auto result_image = render_target->GetColorTexture()->GetImage(true);
      if (create_test_references)
      {
        result_image.Write(reference_image_name);
        std::cout << "Test reference image created " << reference_image_name << std::endl;
      }
      else
      {
        const std::filesystem::path result_image_name = "test-result.png";
        Image2D<Color4f> reference_image(reference_image_name);
        result_image.Write(result_image_name);
        if (!result_image.IsVeryEqual(reference_image))
        {
          tests_succeeded = false;
          std::cerr << "Result image " << result_image_name << " does not match the reference " << reference_image_name
                    << std::endl;
        }
      }
    }

    return Window::ELoopResult::KEEP_LOOPING;
  });

  return tests_succeeded ? EXIT_SUCCESS : EXIT_FAILURE;
}