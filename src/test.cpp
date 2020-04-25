#include "Camera.h"
#include "CameraControllerFly.h"
#include "EBO.h"
#include "FileUtils.h"
#include "Framebuffer.h"
#include "Geometry.h"
#include "Image2D.h"
#include "Macros.h"
#include "Material.h"
#include "Math.h"
#include "Mesh.h"
#include "MeshDrawData.h"
#include "MeshFactory.h"
#include "Renderer.h"
#include "Segment.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Span.h"
#include "StreamOperators.h"
#include "Texture.h"
#include "Texture2D.h"
#include "VAO.h"
#include "VAOVertexAttrib.h"
#include "VBO.h"
#include "VariadicRepeat.h"
#include "Window.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace egl;

int main()
{
  srand(time(0));

  const auto window = std::make_shared<Window>();

  const auto test_mesh = MeshFactory::GetTorus(25, 25, 0.5f);
  const auto texture = std::make_shared<Texture2D>(Image2D { "/home/sephirot47/Downloads/bricks2.jpg" });

  const auto camera = std::make_shared<PerspectiveCamera>();
  camera->SetPosition(Back<Vec3f>() * 12.0f); // Random(All<Vec3f>(0.0f), All<Vec3f>(10.0f)));
  camera->LookAtPoint(Zero<Vec3f>());

  CameraControllerFly camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  const auto render_texture = std::make_shared<Texture2D>(1024, 1024, GL::ETextureInternalFormat::RGBA8);

  const auto circle = MeshFactory::GetCircleSection(40, FullCircleRads());

  auto time = TimeDuration { 0 };
  window->Loop([&](const DeltaTime& inDeltaTime) {
    time += inDeltaTime;

    Renderer renderer;
    renderer.PrepareFor3D(*window);
    {
      render_texture->Resize(window->GetSize());

      if (window->IsKeyPressed(Key::X))
        renderer.SetRenderTexture(render_texture);

      renderer.ClearDepth();
      renderer.ClearBackground(Pink());
      renderer.SetCamera(camera);

      renderer.AddDirectionalLight(Down<Vec3f>(), White<Color3f>());

      const auto q = AngleAxis(time.count() * (0.5f * time.count()), Normalized(Vec3f { 0.0f, 0.0f, 1.0f }));
      // renderer.Rotate(q);
      // renderer.Scale(All<Vec3f>(10.0f));
      renderer.SetLineWidth(3.0f);
      renderer.DrawAxes();

      renderer.GetPerspectiveCamera()->SetAngleOfView(DegreeToRad(60.0f));

      renderer.SetCullFaceEnabled(false);

      const auto obj_pos = Vec3f { 1.0f, 1.0f, 1.0f } * 0.0f;
      renderer.Translate(obj_pos);
      UNUSED(q);
      renderer.Rotate(q);
      renderer.GetMaterial().SetTexture(nullptr);
      renderer.Scale(All<Vec3f>(5.0f));
      renderer.GetMaterial().SetDiffuseColor(White());
      renderer.GetMaterial().SetLightingEnabled(true);
      renderer.GetMaterial().SetSpecularExponent(120.0f);
      renderer.DrawMesh(test_mesh);
      // renderer.DrawMesh(circle);
      renderer.DrawAxes();

      renderer.GetMaterial().SetDiffuseColor(Blue());
      renderer.DrawMesh(test_mesh, Renderer::EDrawType::WIREFRAME);

      renderer.SetPointSize(1.0f);
      renderer.GetMaterial().SetDiffuseColor(Red());
      renderer.DrawMesh(test_mesh, Renderer::EDrawType::POINTS);

      if (window->IsKeyPressed(Key::X))
      {
        renderer.ResetState();
        renderer.SetRenderTexture(nullptr);

        renderer.SetCullFaceEnabled(true);

        RENDERER_STATE_GUARD(renderer, ERendererStateId::CAMERA);
        renderer.GetCamera()->SetPosition(Back<Vec3f>() * 8.0f);
        renderer.GetCamera()->LookAtPoint(Zero<Vec3f>());

        renderer.ClearDepth();
        renderer.ClearBackground(Black());

        renderer.ResetMaterial();
        renderer.Rotate(AngleAxis(0.5f, Forward()));
        renderer.Scale(All<Vec3f>(14.0f));
        renderer.GetMaterial().SetLightingEnabled(false);
        renderer.GetMaterial().SetTexture(render_texture);
        renderer.DrawMesh(MeshFactory::GetPlane());

        const auto triangle = Triangle3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 5.0f, -3.0f), Vec3f(-5.0f, 1.0f, 3.0f));
        renderer.Scale(0.4f);
        renderer.GetMaterial().SetDiffuseColor(Red());
        renderer.DrawTriangle(triangle);
        renderer.GetMaterial().SetDiffuseColor(Blue());
        renderer.SetLineWidth(10.0f);
        renderer.DrawSegments(MakeSpan({ Segment3f { triangle[0], triangle[1] },
            Segment3f { triangle[1], triangle[2] },
            Segment3f { triangle[2], triangle[0] } }));
      }
    }

    renderer.PrepareFor2D(*window);
    {
      RENDERER_STATE_GUARD_ALL(renderer);
      renderer.SetPointSize(15.0f);
      renderer.GetMaterial().SetDiffuseColor(Red());

      // renderer.DrawTriangle();
      renderer.SetDepthTestEnabled(false);
      renderer.DrawPoint(Vec2f(400.0f, 400.0f));
      renderer.DrawSegment(Segment2f { Vec2f { 0.0f, 0.0f }, Vec2f { 500.0f, 800.0f } });

      const auto triangle = Triangle2f(Vec2f(10.0f, 50.0f), Vec2f(50.0f, 100.0f), Vec2f(20.0f, 5.0f));
      renderer.DrawTriangle(triangle);

      renderer.GetMaterial().SetDiffuseColor(Blue());
      renderer.DrawTriangleBoundary(triangle);
    }

    camera_controller_fly.Update(inDeltaTime);
  });
  return EXIT_SUCCESS;
}