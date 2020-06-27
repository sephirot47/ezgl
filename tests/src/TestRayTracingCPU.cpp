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
    camera->SetPosition(Vec3f { 0.0f, 2.0f, 9.0f });
    camera->LookAtPoint(Zero<Vec3f>());
    camera->SetFullAngleOfView(QuarterCircleRads());

    rendererRTCPU.SetCamera(camera);
    rendererRTCPU.SetSize(Vec2ul { 512, 512 });
    rendererRTCPU.AdaptCameraAspectRatio();

    auto scene = MakeSceneGraphNode<ObjectRayTracing>();
    scene->SetLocalTransformation(Transformation3f { Zero<Vec3f>(), AxisAngle(Vec3f { 0.0f, -1.0f, -1.0f }, 0.1f) });
    {
      // Floor
      const auto floor_material = MaterialRayTracing { White<Color4f>() };
      const auto floor_aabox = MakeAAHyperBoxFromCenterHalfSize(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(20.0f, 1.0f, 20.0f));
      const auto floor
          = scene->AddChild(ObjectRayTracing { floor_aabox, floor_material }, Identity<Transformation3f>());

      // Spheres
      scene->AddChild(ObjectRayTracing { Spheref { Zero<Vec3f>(), 1.0f }, MaterialRayTracing { Blue<Color4f>() } },
          Transformation3f { Vec3f { -2.0f, 2.0f, 2.0f }, Identity<Quatf>(), Vec3f { 0.5f, 0.9f, 1.0f } });
      scene->AddChild(ObjectRayTracing { Spheref { Zero<Vec3f>(), 1.0f }, MaterialRayTracing { Pink<Color4f>() } },
          Transformation3f { Vec3f { 2.0f, 2.0f, 2.0f }, Identity<Quatf>(), Vec3f { 0.5f, 2.0f, 1.0f } });
      scene->AddChild(ObjectRayTracing { Spheref { Zero<Vec3f>(), 1.0f }, MaterialRayTracing { Orange<Color4f>() } },
          Transformation3f { Vec3f { 0.0f, -1.0f, 3.0f }, Identity<Quatf>(), Vec3f { 2.0f, 2.0f, 1.0f } });
      scene->AddChild(ObjectRayTracing { Spheref { Zero<Vec3f>(), 1.0f }, MaterialRayTracing { Red<Color4f>() } },
          Transformation3f { Vec3f { 0.0f, 2.0f, -2.0f }, Identity<Quatf>(), Vec3f { 1.0f, 1.0f, 1.0f } });

      // Boxes
      scene->AddChild(ObjectRayTracing { MakeAAHyperBoxFromMinSize(Zero<Vec3f>(), All<Vec3f>(2.0f)),
                          MaterialRayTracing { Purple<Color4f>() } },
          Transformation3f { Vec3f { -3.0f, 2.0f, 0.0f },
              AxisAngle(Vec3f { 0.0f, 1.0f, 0.0f }, 1.0f),
              Vec3f { 0.5f, 0.9f, 1.0f } });
      /*
       */
    }

    // Lights
    DirectionalLight dir_light;
    dir_light.mColor = White<Color3f>() * 0.5f;
    dir_light.mDirection = NormalizedSafe(Vec3f(0.0f, -1.3f, -2.0f));
    scene->AddChild(ObjectRayTracing { dir_light });

    rendererRTCPU.DrawScene(*scene);
  }

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