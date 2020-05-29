#include "ez/CameraControllerFly.h"
#include "ez/Font.h"
#include "ez/Image2D.h"
#include "ez/MeshFactory.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Renderer3D.h"
#include "ez/RendererRayTracingCPU.h"
#include "ez/SceneGraphNode.h"
#include "ez/Window.h"
#include <cstdlib>

using namespace ez;

int main(int argc, const char** argv)
{
  srand(1234);

  RendererRayTracingCPU rendererRTCPU;
  {
    const auto camera = std::make_shared<PerspectiveCameraf>();
    camera->SetPosition(Back<Vec3f>() * 10.0f);
    camera->LookAtPoint(Zero<Vec3f>());
    camera->SetFullAngleOfView(QuarterCircleRads());

    rendererRTCPU.SetCamera(camera);
    rendererRTCPU.SetSize(Vec2ul { 512, 512 });
    rendererRTCPU.AdaptCameraAspectRatio();

    auto scene = MakeSceneGraphNode<DrawableRayTracing>();

    // scene.AddChild(DrawableRayTracing { Spheref(One<Vec3f>(), 1.0f) }, Identity<Transformation3f>());
    // scene.AddChild(DrawableRayTracing { Spheref(Zero<Vec3f>(), 2.0f) }, Identity<Transformation3f>());

    for (int i = 0; i < 100; ++i)
    {
      const auto material_raytracing = MaterialRayTracing { XYZ1(HSVToRGB(Color3f(RandomUnit<float>(), 1.0f, 1.0f))) };
      const auto transformation = Transformation3f { Random<Vec3f>(All<Vec3f>(-5.0f), All<Vec3f>(5.0f)),
        AxisAngle(NormalizedSafe(RandomUnit<Vec3f>()), Random<float>(0.0f, 4.0f)),
        Random<Vec3f>(All<Vec3f>(0.05f), All<Vec3f>(0.3f)) };

      if (i % 2 == 0)
      {
        const auto sphere = Spheref(Zero<Vec3f>(), 2.0f);
        scene->AddChild(DrawableRayTracing { sphere, material_raytracing }, transformation);
      }
      else
      {
        const auto box = MakeAAHyperBoxFromCenterHalfSize(RandomUnit<Vec3f>() * 5.0f, RandomUnit<Vec3f>() * 5.0f);
        scene->AddChild(DrawableRayTracing { box, material_raytracing }, transformation);
      }
    }
    rendererRTCPU.DrawScene(*scene);
  }

  rendererRTCPU.GetRenderTarget().Write("test.png");

  // Present results in Window
  {
    Window::CreateOptions window_create_options;
    window_create_options.mTitle = "Test RayTracing CPU";
    const auto window = std::make_shared<Window>(window_create_options);

    const auto camera = std::make_shared<OrthographicCamera3f>();
    camera->SetOrthoMin(Vec3f { -0.5f, -0.5f, -1.0f });
    camera->SetOrthoMax(Vec3f { 0.5f, 0.5f, 1.0f });
    camera->SetPosition(Back<Vec3f>());
    camera->LookAtPoint(Zero<Vec3f>());

    Renderer3D renderer3D;
    const auto result_texture = std::make_shared<Texture2D>(rendererRTCPU.GetRenderTarget());
    const auto plane_mesh = MeshFactory::GetPlane();

    window->Loop([&](const DeltaTime& inDeltaTime) {
      GL::Viewport(Zero<Vec2i>(), window->GetFramebufferSize());

      // Prepare renderers
      renderer3D.ResetState();
      renderer3D.SetCamera(camera);
      renderer3D.AdaptToWindow(*window);
      renderer3D.Clear(Gray<Color4f>());

      renderer3D.SetCullFaceEnabled(false);
      renderer3D.GetMaterial().SetLightingEnabled(false);

      renderer3D.GetMaterial().SetTexture(result_texture);
      renderer3D.DrawMesh(plane_mesh);

      // Blit image to window
      GL::ClearDepth();
      renderer3D.Blit();

      // camera_controller_fly.Update(inDeltaTime);

      return Window::ELoopResult::KEEP_LOOPING;
    });
  }

  return EXIT_SUCCESS;
}