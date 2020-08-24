#include "TestSegmentController.h"
#include <ez/AAHyperBox.h>
#include <ez/BinaryIndex.h>
#include <ez/CameraControllerFly.h>
#include <ez/Capsule.h>
#include <ez/HyperSphere.h>
#include <ez/MathRandom.h>
#include <ez/OrthographicCamera.h>
#include <ez/PerspectiveCamera.h>
#include "ez/Renderer3D.h"
#include <ez/Segment.h>
#include <ez/TupleForEach.h>
#include <ez/Window.h>
#include <array>
#include <cstdlib>

using namespace ez;

int main(int argc, const char** argv)
{
  srand(1234);

  const auto aabox = MakeAAHyperBoxFromCenterSize(Vec3f { -1.5f, 0.0f, 0.0f }, Vec3f { 1.0f, 1.5f, 1.8f });
  const auto sphere = Spheref { Vec3f { 1.5f, -0.2f, 0.1f }, 0.7f };
  const auto cylinder = Cylinderf { Vec3f { -1.0f, -1.5f, -0.6f }, Vec3f { 1.5f, -3.0f, 0.1f }, 0.4f };
  const auto capsule = Capsule3f { Vec3f { 0.4f, -0.9f, 0.9f }, Vec3f { 1.3f, -2.2f, -1.0f }, 0.3f };
  const auto plane = Planef { Normalized(Vec3f { 0.1f, 1.0f, -0.2 }), Vec3f { 1.0f, -4.0f, -1.0f } };
  const auto primitives = std::make_tuple(aabox, sphere, aacube, cylinder, capsule, plane);

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Segment intersection 3D";
  const auto window = std::make_shared<Window>(window_create_options);

  TestSegmentController<3, 128> segment_controller;
  window->AddInputListener(&segment_controller);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(Back<Vec3f>() * 5.0f);
  camera->LookAtPoint(Zero<Vec3f>());

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);
  camera_controller_fly.GetParameters().mMinFlySpeed = 0.1f;
  camera_controller_fly.GetParameters().mMaxFlySpeed = 1.5f;
  camera_controller_fly.ApplyParameters();

  // Create renderer
  Renderer3D renderer;

  // Window loop
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    renderer.Clear();

    renderer.AddDirectionalLight(NormalizedSafe(Vec3f { -1.0f, -2.0f, -0.5f }), White<Color3f>());

    if (!segment_controller.IsControlEnabled())
    {
      camera_controller_fly.Update(inDeltaTime);
    }

    const auto segments = segment_controller.GetSegments();
    for (const auto& segment : segments)
    {
      auto intersecting = false;
      ForEach(primitives, [&](const auto& in_primitive) { intersecting |= IntersectCheck(segment, in_primitive); });

      renderer.SetLineWidth(2.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
      renderer.GetMaterial().SetDiffuseColor(intersecting ? Red<Color4f>() : Green<Color4f>());
      renderer.Draw(segment);

      renderer.SetPointSize(8.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
      ForEach(primitives, [&](const auto& in_primitive) {
        renderer.GetMaterial().SetDiffuseColor(Red<Color4f>());
        const auto intersection_distances = IntersectAll(segment, in_primitive);
        for (const auto& intersection_distance : intersection_distances)
        {
          if (!intersection_distance)
            continue;

          const auto intersection_point = (segment.GetOrigin() + Direction(segment) * (*intersection_distance));
          renderer.Draw(intersection_point);
        }

        renderer.GetMaterial().SetDiffuseColor(Blue<Color4f>());
        const auto closest_intersection_distance = IntersectClosest(segment, in_primitive);
        if (closest_intersection_distance)
          renderer.Draw((segment.GetOrigin() + Direction(segment) * (*closest_intersection_distance)));
      });
    }

    renderer.GetMaterial().SetDiffuseColor(WithAlpha(White<Color4f>(), 0.5f));
    renderer.GetMaterial().SetLightingEnabled(true);
    renderer.SetDepthFunc(GL::EDepthFunc::LEQUAL);
    renderer.SetBlendFunc(GL::EBlendFactor::SRC_ALPHA, GL::EBlendFactor::ONE_MINUS_SRC_ALPHA);
    ForEach(primitives, [&](const auto& in_primitive) { renderer.Draw(in_primitive); });

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });
  return EXIT_SUCCESS;
}