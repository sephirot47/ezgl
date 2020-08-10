#include "TestSegmentController.h"
#include "ez/AABox.h"
#include "ez/AACube.h"
#include "ez/BinaryIndex.h"
#include "ez/CameraControllerFly.h"
#include "ez/Capsule.h"
#include "ez/HyperSphere.h"
#include "ez/MathRandom.h"
#include "ez/OrthographicCamera.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Ray.h"
#include "ez/Renderer2D.h"
#include "ez/Segment.h"
#include "ez/TupleForEach.h"
#include "ez/Window.h"
#include <array>
#include <cstdlib>

using namespace ez;

int main(int argc, const char** argv)
{
  srand(1234);

  const auto aarect = MakeAAHyperBoxFromCenterSize(Vec2f { -1.5f, 0.0f }, Vec2f { 1.0f, 1.5f });
  const auto aasquare = AASquaref { Vec2f { -1.0f, -2.0f }, 0.5f };
  const auto circle = Circlef { Vec2f { 1.0f, 0.0f }, 0.5f };
  const auto primitives = std::make_tuple(aarect, aasquare, circle);

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Segment AABox intersection 2D";
  const auto window = std::make_shared<Window>(window_create_options);

  TestSegmentController<2> segment_controller;
  window->AddInputListener(&segment_controller);

  // Camera
  const auto camera = std::make_shared<OrthographicCamera2f>();

  // Camera controller
  CameraControllerFly2f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);
  camera_controller_fly.GetParameters().mMinFlySpeed = 0.1f;
  camera_controller_fly.GetParameters().mMaxFlySpeed = 1.5f;
  camera_controller_fly.ApplyParameters();

  // Create renderer
  Renderer2D renderer;

  // Window loop
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    camera->SetOrthoMin(-5.0f * One<Vec2f>());
    camera->SetOrthoMax(5.0f * One<Vec2f>());
    renderer.Clear();

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
      renderer.GetMaterial().SetColor(intersecting ? Red<Color4f>() : Green<Color4f>());
      renderer.Draw(segment);

      renderer.SetPointSize(8.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
      ForEach(primitives, [&](const auto& in_primitive) {
        renderer.GetMaterial().SetColor(Red<Color4f>());
        const auto intersection_distances = IntersectAll(segment, in_primitive);
        for (const auto& intersection_distance : intersection_distances)
        {
          if (!intersection_distance)
            continue;

          const auto intersection_point = (segment.GetOrigin() + Direction(segment) * (*intersection_distance));
          renderer.Draw(intersection_point);
        }

        renderer.GetMaterial().SetColor(Blue<Color4f>());
        const auto closest_intersection_distance = IntersectClosest(segment, in_primitive);
        if (closest_intersection_distance)
          renderer.Draw((segment.GetOrigin() + Direction(segment) * (*closest_intersection_distance)));
      });
    }

    renderer.GetMaterial().SetColor(WithAlpha(White<Color4f>(), 0.5f));
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