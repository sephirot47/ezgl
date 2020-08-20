#include "TestSegmentController.h"
#include "ez/AAHyperBox.h"
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
  // const auto aarect = MakeAAHyperBoxFromCenterSize(Vec2f { -1.5f, 0.0f }, Vec2f { 1.0f, 1.5f });
  // const auto circle = Circlef { Vec2f { 1.0f, 0.0f }, 0.5f };
  const auto other_segment = Segment2f { Vec2f { 1.0f, 1.3f }, Vec2f { -1.1f, 2.4f } };
  const auto primitives = std::make_tuple(other_segment); // aarect, circle, other_segment);

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Segment distance 2D";
  const auto window = std::make_shared<Window>(window_create_options);

  TestSegmentController<2, 1> segment_controller;
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
      renderer.SetLineWidth(2.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
      renderer.GetMaterial().SetColor(Green<Color4f>());
      renderer.Draw(segment);

      auto min_distance = Max<float>();
      ForEach(primitives, [&](const auto& in_primitive) {
        min_distance = Min(Distance(segment, in_primitive), min_distance);
        const auto closest_point_0 = ClosestPoint(segment, in_primitive);
        const auto closest_point_1 = ClosestPoint(in_primitive, segment);
        renderer.GetMaterial().SetColor(Purple<Color4f>());
        renderer.Draw(closest_point_0);
        renderer.Draw(closest_point_1);
        renderer.Draw(Segment2f { closest_point_0, closest_point_1 });
      });

      if (min_distance > 0.0f && min_distance != Max<float>())
      {
        const auto distance_capsule = Capsule2f { segment.GetOrigin(), segment.GetDestiny(), min_distance };
        renderer.SetDepthFunc(GL::EDepthFunc::LEQUAL);
        renderer.GetMaterial().SetColor(WithAlpha(Cyan<Color4f>(), 0.25f));
        renderer.DrawCapsule(distance_capsule, 32);
      }

      renderer.SetPointSize(8.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
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