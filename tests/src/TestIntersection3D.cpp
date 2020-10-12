#include "TestPrimitiveController.h"
#include <array>
#include <cstdlib>
#include <ez/AAHyperBox.h>
#include <ez/BinaryIndex.h>
#include <ez/CameraControllerFly.h>
#include <ez/Capsule.h>
#include <ez/Font.h>
#include <ez/HyperBox.h>
#include <ez/HyperSphere.h>
#include <ez/MathRandom.h>
#include <ez/OrthographicCamera.h>
#include <ez/PerspectiveCamera.h>
#include <ez/Ray.h>
#include <ez/Renderer3D.h>
#include <ez/Segment.h>
#include <ez/TupleForEach.h>
#include <ez/Window.h>

using namespace ez;

template <typename TPrimitivesControllersTuple>
std::string GetPrimitiveName(const TPrimitivesControllersTuple& inPrimitivesControllersTuple,
    const int inSelectedPrimitiveIndex);

int main(int argc, const char** argv)
{
  srand(1234);

  constexpr auto NumLines = 64;
  const auto point = Vec3f { 3.0f, -0.5f, 0.0f };
  const auto line = Line3f { Vec3f { -1.0f, 3.0f, -3.0f }, Direction(Vec3f { 1.0f, 2.0f, 3.0f }) };
  const auto ray = Ray { Vec3f { -3.5f, 1.0f, 0.0f }, Direction(Vec3f { -1.1f, 3.3f, 0.0f }) };
  const auto segment = Segment { Vec3f { 1.0f, 1.3f, 0.0f }, Vec3f { -2.6f, 3.4f, 0.0f } };
  const auto plane = Plane { Normalized(Vec3f { 1.0f, 1.3f, -0.1f }), Vec3f { 1.0f, 2.5f, -1.0f } };
  const auto sphere = Spheref { Vec3f { 1.0f, -3.0f, 1.0f }, 1.337f };
  const auto aabox = MakeAAHyperBoxFromCenterSize(Vec3f { -1.5f, 0.0f, 1.0f }, Vec3f { 1.0f, 1.5f, 3.0f });
  const auto box = Boxf { Vec3f { -2.0f, -2.0f, -2.0f },
    Vec3f { 2.0f, 1.5f, 1.0f },
    AngleAxis(0.5f, NormalizedSafe(Vec3f { 1.0f, 0.5f, -0.2f })) };
  const auto cylinder = Cylinderf { Vec3f { 1.0f, 2.0f, -2.0f }, Vec3f { 1.7f, 1.6f, -1.5f }, 0.5f };
  const auto capsule = Capsule3f { Vec3f { 1.0f, 1.0f, 1.0f }, Vec3f { 1.7f, 0.6f, 2.0f }, 0.3f };
  const auto triangle
      = Triangle3f { Vec3f { 0.1f, -3.0f, 2.0f }, Vec3f { 1.7f, -3.7f, 1.3f }, Vec3f { 0.3f, -4.7f, 0.1f } };
  const auto primitives
      = std::make_tuple(point, line, ray, segment, plane, sphere, aabox, box); // , cylinder, capsule, triangle);
  auto main_primitives_controllers = std::make_tuple(TestPrimitiveController<Line3f, NumLines> {},
      TestPrimitiveController<Ray3f, NumLines> {},
      TestPrimitiveController<Segment3f, NumLines> {},
      TestPrimitiveController<Planef> {},
      TestPrimitiveController<Spheref> {},
      TestPrimitiveController<AABoxf> {},
      TestPrimitiveController<Boxf> {} // TestPrimitiveController<Cylinderf> {},
                                       // TestPrimitiveController<Capsule3f> {},
                                       // TestPrimitiveController<Triangle3f> {}
  );
  constexpr int NumMainPrimitives = std::tuple_size<decltype(main_primitives_controllers)>();
  int selected_main_primitive_index = 0;
  auto see_closest_points = false;

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test intersection 3D";
  const auto window = std::make_shared<Window>(window_create_options);
  window->AddKeyEventCallback([&](const KeyEvent& inKeyEvent) {
    if (inKeyEvent.IsPressOrRepeat())
    {
      if (inKeyEvent.mKey == EKey::LEFT)
        --selected_main_primitive_index;
      else if (inKeyEvent.mKey == EKey::RIGHT)
        ++selected_main_primitive_index;
      else if (inKeyEvent.mKey == EKey::C)
        see_closest_points = !see_closest_points;
      selected_main_primitive_index = (selected_main_primitive_index + NumMainPrimitives) % NumMainPrimitives;
    }
  });

  const Font font("/home/sephirot47/ezgl/res/Fonts/Ubuntu-Regular.ttf", 128);

  ForEach(main_primitives_controllers,
      [&](auto& in_main_primitive_controller) { window->AddInputListener(&in_main_primitive_controller); });

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(Vec3f { 0.0f, 0.0f, 5.0f });

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);
  camera_controller_fly.GetParameters().mMinFlySpeed = 0.3f;
  camera_controller_fly.GetParameters().mMaxFlySpeed = 2.5f;
  camera_controller_fly.ApplyParameters();

  // Create renderer
  Renderer3D renderer;

  // Window loop
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    renderer.Clear();

    renderer.AddDirectionalLight(Normalized(Vec3f { 1.0f, -2.0f, -0.3f }), White<Color3f>());

    int main_primitive_i = 0;
    ForEach(main_primitives_controllers, [&](const auto& in_main_primitive_controller) {
      if (main_primitive_i++ != selected_main_primitive_index)
        return;

      if (!window->IsKeyPressed(EKey::LEFT_ALT) && !window->IsKeyPressed(EKey::LEFT_CONTROL)
          && !window->IsKeyPressed(EKey::LEFT_SHIFT))
      {
        camera_controller_fly.Update(inDeltaTime);
      }

      const auto main_subprimitives = in_main_primitive_controller.GetPrimitives();
      for (const auto main_subprimitive : main_subprimitives)
      {
        using MainPrimitiveType = typename std::remove_cvref_t<decltype(in_main_primitive_controller)>::PrimitiveType;

        auto intersecting = false;
        auto contained = false;
        ForEach(primitives, [&](const auto& in_primitive) {
          using PrimitiveType = std::remove_cvref_t<decltype(in_primitive)>;
          intersecting |= IntersectCheck(main_subprimitive, in_primitive);
          if constexpr (!IsVec_v<PrimitiveType>)
            contained |= Contains(in_primitive, main_subprimitive);
        });

        renderer.SetLineWidth(2.0f);
        renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
        renderer.GetMaterial().SetDiffuseColor(
            contained ? Red<Color4f>() : (intersecting ? Yellow<Color4f>() : Green<Color4f>()));
        renderer.GetMaterial().SetLightingEnabled(true);
        renderer.Draw(main_subprimitive);

        if constexpr (IsLine_v<MainPrimitiveType> || IsRay_v<MainPrimitiveType> || IsSegment_v<MainPrimitiveType>)
        {
          // Intersection point
          renderer.SetPointSize(8.0f);
          renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
          ForEach(primitives, [&](const auto& in_primitive) {
            renderer.GetMaterial().SetDiffuseColor(Red<Color4f>());
            const auto intersection_distances = IntersectAll(main_subprimitive, in_primitive);
            for (const auto& intersection_distance : intersection_distances)
            {
              if (!intersection_distance)
                continue;

              const auto intersection_point
                  = (main_subprimitive.GetOrigin() + Direction(main_subprimitive) * (*intersection_distance));
              renderer.Draw(intersection_point);
            }

            renderer.GetMaterial().SetDiffuseColor(Blue<Color4f>());
            const auto closest_intersection_distance = IntersectClosest(main_subprimitive, in_primitive);
            if (closest_intersection_distance)
              renderer.Draw(
                  (main_subprimitive.GetOrigin() + Direction(main_subprimitive) * (*closest_intersection_distance)));
          });
        }
        else if (see_closest_points)
        {
          // ClosestPoint
          ForEach(primitives, [&](const auto& in_primitive) {
            if (IntersectCheck(main_subprimitive, in_primitive))
              return;

            const auto closest_point_in_main_subprimitive = ClosestPoint(main_subprimitive, in_primitive);
            const auto closest_point_in_primitive = ClosestPoint(in_primitive, closest_point_in_main_subprimitive);

            renderer.GetMaterial().SetDiffuseColor(Purple<Color4f>());
            renderer.Draw(closest_point_in_main_subprimitive);
            renderer.Draw(closest_point_in_primitive);
            renderer.Draw(Segment3f { closest_point_in_main_subprimitive, closest_point_in_primitive });
          });
        }
      }
    });

    renderer.GetMaterial().SetDiffuseColor(White<Color4f>());
    renderer.SetDepthFunc(GL::EDepthFunc::LEQUAL);
    renderer.SetBlendFunc(GL::EBlendFactor::SRC_ALPHA, GL::EBlendFactor::ONE_MINUS_SRC_ALPHA);
    ForEach(primitives, [&](const auto& in_primitive) { renderer.Draw(in_primitive); });

    /*
    renderer.PushTransformMatrix();
    renderer.Translate(Vec3f { -5.0f, -5.0f } + Vec3f { 0.1f, 0.1f });
    renderer.GetMaterial().SetDiffuseColor(White<Color4f>());
    renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);

    const auto primitive_name = GetPrimitiveName(main_primitives_controllers, selected_main_primitive_index);
    const std::string text = "Left/Right: change primitive (Current: " + primitive_name
        + ")\n"
          "WASD: Move camera\n"
          "Ctrl + WASD: Move primitive\n"
          "Shift + WASD: Rotate primitive\n"
          "Alt + WASD: Resize primitive\n"
          "C: Toggle see closest points lines ("
        + (see_closest_points ? "ON" : "OFF")
        + ")\n"
          "Green = Not-Intersecting, Yellow = Intersecting, Red = Contained";
    renderer.DrawText(text, font, 0.002f, ETextHAlignment::LEFT, ETextVAlignment::BOTTOM);
    renderer.PopTransformMatrix();
    */

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });
  return EXIT_SUCCESS;
}

template <typename TPrimitivesControllersTuple>
std::string GetPrimitiveName(const TPrimitivesControllersTuple& inPrimitivesControllersTuple,
    const int inSelectedPrimitiveIndex)
{
  std::string primitive_name = "Unknown";
  int main_primitive_i = 0;
  ForEach(inPrimitivesControllersTuple, [&](const auto& in_main_primitive_controller) {
    using MainPrimitiveType = typename std::remove_cvref_t<decltype(in_main_primitive_controller)>::PrimitiveType;
    if (main_primitive_i++ != inSelectedPrimitiveIndex)
      return;
    if constexpr (IsLine_v<MainPrimitiveType>)
      primitive_name = "Line";
    if constexpr (IsRay_v<MainPrimitiveType>)
      primitive_name = "Ray";
    if constexpr (IsSegment_v<MainPrimitiveType>)
      primitive_name = "Segment";
    if constexpr (IsHyperSphere_v<MainPrimitiveType>)
      primitive_name = "Circle";
    if constexpr (IsAAHyperBox_v<MainPrimitiveType>)
      primitive_name = "AARect";
    if constexpr (IsHyperBox_v<MainPrimitiveType>)
      primitive_name = "Rect";
    if constexpr (IsCapsule_v<MainPrimitiveType>)
      primitive_name = "Capsule";
    if constexpr (IsTriangle_v<MainPrimitiveType>)
      primitive_name = "Triangle";
  });
  return primitive_name;
}