#include "TestPrimitiveController.h"
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
  const auto segment = Segment2f { Vec2f { 1.0f, 1.3f }, Vec2f { -1.1f, 2.4f } };
  const auto primitives = std::make_tuple(aarect, aasquare, circle, segment);
  auto main_primitives_controllers = std::make_tuple(TestPrimitiveController<Segment2f, 128> {});

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test intersection 2D";
  const auto window = std::make_shared<Window>(window_create_options);

  ForEach(main_primitives_controllers,
      [&](auto& main_primitive_controller) { window->AddInputListener(&main_primitive_controller); });

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
  const auto& selected_main_primitive_controller = std::get<0>(main_primitives_controllers);
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    camera->SetOrthoMin(-5.0f * One<Vec2f>());
    camera->SetOrthoMax(5.0f * One<Vec2f>());
    renderer.Clear();

    if (!selected_main_primitive_controller.IsControlEnabled())
    {
      camera_controller_fly.Update(inDeltaTime);
    }

    const auto main_primitives = selected_main_primitive_controller.GetPrimitives();
    for (const auto& main_primitive : main_primitives)
    {
      auto intersecting = false;
      ForEach(primitives,
          [&](const auto& in_primitive) { intersecting |= IntersectCheck(main_primitive, in_primitive); });

      renderer.SetLineWidth(2.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
      renderer.GetMaterial().SetColor(intersecting ? Red<Color4f>() : Green<Color4f>());
      renderer.Draw(main_primitive);

      renderer.SetPointSize(8.0f);
      renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
      ForEach(primitives, [&](const auto& in_primitive) {
        renderer.GetMaterial().SetColor(Red<Color4f>());
        const auto intersection_distances = IntersectAll(main_primitive, in_primitive);
        for (const auto& intersection_distance : intersection_distances)
        {
          if (!intersection_distance)
            continue;

          const auto intersection_point
              = (main_primitive.GetOrigin() + Direction(main_primitive) * (*intersection_distance));
          renderer.Draw(intersection_point);
        }

        renderer.GetMaterial().SetColor(Blue<Color4f>());
        const auto closest_intersection_distance = IntersectClosest(main_primitive, in_primitive);
        if (closest_intersection_distance)
          renderer.Draw((main_primitive.GetOrigin() + Direction(main_primitive) * (*closest_intersection_distance)));
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