#include "TestPrimitiveController.h"
#include "ez/AAHyperBox.h"
#include "ez/BinaryIndex.h"
#include "ez/CameraControllerFly.h"
#include "ez/Capsule.h"
#include "ez/HyperBox.h"
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

  const auto point = Vec2f { 3.0f, 1.0f };
  const auto line = Line2f { Vec2f { 5.5f, 1.0f }, Direction(Vec2f { 1.1f, -0.2f }) };
  const auto ray = Ray2f { Vec2f { -3.5f, 1.0f }, Direction(Vec2f { -1.1f, 3.2f }) };
  const auto segment = Segment2f { Vec2f { 1.0f, 1.3f }, Vec2f { -2.6f, 3.4f } };
  const auto circle = Circlef { Vec2f { 1.0f, 0.0f }, 0.5f };
  const auto aarect = MakeAAHyperBoxFromCenterSize(Vec2f { -1.5f, 0.0f }, Vec2f { 1.0f, 1.5f });
  const auto rect = Rectf { Vec2f { -2.0f, -2.0f }, Vec2f { 1.0f, 1.5f } * 0.5f, 0.57f };
  const auto capsule = Capsule2f { Vec2f { 1.0f, -2.0f }, Vec2f { 1.7f, -1.6f }, 0.3f };
  const auto triangle = Triangle2f { Vec2f { 0.1f, 0.4f }, Vec2f { 0.7f, -0.1f }, Vec2f { 0.3f, 0.9f } };
  const auto primitives = std::make_tuple(point); // , line, ray, segment, circle, aarect, rect, capsule); //, triangle);
  auto main_primitives_controllers = std::make_tuple(TestPrimitiveController<Line2f, 128> {},
      TestPrimitiveController<Ray2f, 128> {},
      TestPrimitiveController<Segment2f, 128> {},
      TestPrimitiveController<Circlef> {},
      TestPrimitiveController<AARectf> {},
      TestPrimitiveController<Rectf> {},
      TestPrimitiveController<Capsule2f> {},
      TestPrimitiveController<Triangle2f> {});
  constexpr int NumMainPrimitives = std::tuple_size<decltype(main_primitives_controllers)>();
  int selected_main_primitive_index = 0;

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test intersection 2D";
  const auto window = std::make_shared<Window>(window_create_options);
  window->SetInputEventCallback([&](const InputEvent& inInputEvent) {
    if (inInputEvent.GetType() == InputEvent::EType::KEY)
    {
      const auto key_event = inInputEvent.As<InputEvent::EType::KEY>();
      if (key_event.IsPressOrRepeat())
      {
        if (key_event.mKey == EKey::LEFT)
          --selected_main_primitive_index;
        else if (key_event.mKey == EKey::RIGHT)
          ++selected_main_primitive_index;
        selected_main_primitive_index = (selected_main_primitive_index + NumMainPrimitives) % NumMainPrimitives;
      }
    }
  });

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
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    camera->SetOrthoMin(-5.0f * One<Vec2f>());
    camera->SetOrthoMax(5.0f * One<Vec2f>());
    renderer.Clear();

    int main_primitive_i = 0;
    ForEach(main_primitives_controllers, [&](const auto& main_primitive_controller) {
      if (main_primitive_i++ != selected_main_primitive_index)
        return;

      if (!main_primitive_controller.IsControlEnabled())
        camera_controller_fly.Update(inDeltaTime);

      const auto main_subprimitives = main_primitive_controller.GetPrimitives();
      for (const auto main_subprimitive : main_subprimitives)
      {
        using MainPrimitiveType = typename std::remove_cvref_t<decltype(main_primitive_controller)>::PrimitiveType;

        auto intersecting = false;
        ForEach(primitives,
            [&](const auto& in_primitive) { intersecting |= IntersectCheck(main_subprimitive, in_primitive); });

        renderer.SetLineWidth(2.0f);
        renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
        renderer.GetMaterial().SetColor(intersecting ? Red<Color4f>() : Green<Color4f>());
        renderer.Draw(main_subprimitive);

        if constexpr (IsLine_v<MainPrimitiveType> || IsRay_v<MainPrimitiveType> || IsSegment_v<MainPrimitiveType>)
        {
          renderer.SetPointSize(8.0f);
          renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
          ForEach(primitives, [&](const auto& in_primitive) {
            renderer.GetMaterial().SetColor(Red<Color4f>());
            const auto intersection_distances = IntersectAll(main_subprimitive, in_primitive);
            for (const auto& intersection_distance : intersection_distances)
            {
              if (!intersection_distance)
                continue;

              const auto intersection_point
                  = (main_subprimitive.GetOrigin() + Direction(main_subprimitive) * (*intersection_distance));
              renderer.Draw(intersection_point);
            }

            renderer.GetMaterial().SetColor(Blue<Color4f>());
            const auto closest_intersection_distance = IntersectClosest(main_subprimitive, in_primitive);
            if (closest_intersection_distance)
              renderer.Draw(
                  (main_subprimitive.GetOrigin() + Direction(main_subprimitive) * (*closest_intersection_distance)));
          });
        }
      }
    });

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