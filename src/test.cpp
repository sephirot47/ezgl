#include "Camera.h"
#include "CameraControllerFly.h"
#include "EBO.h"
#include "FileUtils.h"
#include "Framebuffer.h"
#include "Geometry.h"
#include "Image2D.h"
#include "Macros.h"
#include "Material3D.h"
#include "Math.h"
#include "Mesh.h"
#include "MeshDrawData.h"
#include "MeshFactory.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
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

  const auto camera3d = std::make_shared<PerspectiveCameraf>();
  camera3d->SetPosition(Back<Vec3f>() * 12.0f); // Random(All<Vec3f>(0.0f), All<Vec3f>(10.0f)));
  camera3d->LookAtPoint(Zero<Vec3f>());

  const auto camera2d = std::make_shared<OrthographicCamera2f>();

  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera3d);
  camera_controller_fly.SetWindow(window);

  const auto render_texture = std::make_shared<Texture2D>(1024, 1024, GL::ETextureInternalFormat::RGBA8);

  auto time = TimeDuration { 0 };
  window->Loop([&](const DeltaTime& inDeltaTime) {
    time += inDeltaTime;

    Renderer3D renderer3D;
    renderer3D.Begin(*window);
    {
      if (window->IsKeyPressed(Key::X))
        renderer3D.SetRenderTexture(render_texture);

      renderer3D.SetLineWidth(5.0f);
      renderer3D.Clear(Pink());
      render_texture->Resize(window->GetSize());

      renderer3D.SetCamera(camera3d);
      renderer3D.AddDirectionalLight(Down<Vec3f>(), White<Color3f>());

      const auto q = AngleAxis(time.count() * (0.5f * time.count()), Normalized(Vec3f { 0.0f, 0.0f, 1.0f }));
      UNUSED(q);

      const auto obj_pos = Vec3f { 1.0f, 1.0f, 1.0f } * 0.0f;
      renderer3D.Translate(obj_pos);
      renderer3D.Rotate(q);
      renderer3D.GetMaterial().SetTexture(nullptr);
      renderer3D.Scale(All<Vec3f>(5.0f));
      renderer3D.GetMaterial().SetDiffuseColor(White());
      renderer3D.GetMaterial().SetLightingEnabled(true);
      renderer3D.GetMaterial().SetSpecularExponent(120.0f);
      renderer3D.DrawMesh(test_mesh);
      // renderer3D.DrawMesh(circle);
      renderer3D.DrawAxes();
      renderer3D.GetMaterial().SetDiffuseColor(Blue());
      renderer3D.DrawMesh(test_mesh, Renderer::EDrawType::WIREFRAME);

      renderer3D.SetPointSize(1.0f);
      renderer3D.GetMaterial().SetDiffuseColor(Red());
      renderer3D.DrawMesh(test_mesh, Renderer::EDrawType::POINTS);

      if (window->IsKeyPressed(Key::X))
      {
        renderer3D.ResetState();
        renderer3D.SetRenderTexture(nullptr);

        RENDERER_STATE_GUARD(renderer3D, Renderer3D::EStateId::CAMERA);
        renderer3D.GetCamera()->SetPosition(Back<Vec3f>() * 8.0f);
        renderer3D.GetCamera()->LookAtPoint(Zero<Vec3f>());

        renderer3D.ResetMaterial();
        renderer3D.Rotate(AngleAxis(0.5f, Forward()));
        renderer3D.Scale(All<Vec3f>(14.0f));
        renderer3D.GetMaterial().SetLightingEnabled(false);
        renderer3D.GetMaterial().SetTexture(render_texture);
        renderer3D.DrawMesh(MeshFactory::GetPlane());

        const auto triangle = Triangle3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 5.0f, -3.0f), Vec3f(-5.0f, 1.0f, 3.0f));
        renderer3D.Scale(0.4f);
        renderer3D.GetMaterial().SetDiffuseColor(Red());
        renderer3D.DrawTriangle(triangle);
        renderer3D.GetMaterial().SetDiffuseColor(Blue());
        renderer3D.SetLineWidth(10.0f);
        renderer3D.DrawSegments(MakeSpan({ Segment3f { triangle[0], triangle[1] },
            Segment3f { triangle[1], triangle[2] },
            Segment3f { triangle[2], triangle[0] } }));
      }
    }

    Renderer2D renderer2D;
    renderer2D.Begin(*window);
    {
      renderer2D.SetLineWidth(3.0f);

      RENDERER_STATE_GUARD_ALL(renderer2D);
      renderer2D.SetCamera(camera2d);
      renderer2D.AdaptCameraToWindow(*window);
      renderer2D.SetPointSize(15.0f);
      renderer2D.GetMaterial().SetColor(Red());

      // renderer2D.DrawTriangle();
      renderer2D.DrawPoint(Vec2f(400.0f, 400.0f));
      renderer2D.DrawSegment(Segment2f { Vec2f { 0.0f, 0.0f }, Vec2f { 500.0f, 800.0f } });

      const auto triangle = Triangle2f(Vec2f(10.0f, 50.0f), Vec2f(50.0f, 100.0f), Vec2f(20.0f, 5.0f));
      renderer2D.DrawTriangle(triangle);

      renderer2D.GetMaterial().SetColor(Blue());
      renderer2D.DrawTriangleBoundary(triangle);
    }
    /*
     */

    camera_controller_fly.Update(inDeltaTime);
  });
  return EXIT_SUCCESS;
}