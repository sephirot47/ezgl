#include "ez/AABox.h"
#include "ez/AACube.h"
#include "ez/BinaryIndex.h"
#include "ez/CameraControllerFly.h"
#include "ez/Capsule.h"
#include "ez/HyperSphere.h"
#include "ez/MathRandom.h"
#include "ez/OrthographicCamera.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Renderer3D.h"
#include "ez/Segment.h"
#include "ez/TupleForEach.h"
#include "ez/Window.h"
#include <array>
#include <cstdlib>

using namespace ez;

class TestSegmentController : public InputListener
{
public:
  static constexpr auto MinLength = 0.1f;

  void OnInput(const InputEvent& inInputEvent) override
  {
    if (inInputEvent.GetType() == InputEvent::EType::KEY)
    {
      const auto key_event = inInputEvent.As<InputEvent::EType::KEY>();
      if (key_event.IsPressOrRepeat())
      {
        if (key_event.mKey == EKey::TAB)
          mControlEnabled = !mControlEnabled;

        if (!IsControlEnabled())
          return;

        if (key_event.IsShiftModifierPressed())
        {
          const auto delta_angle = QuarterCircleRads<float>() * 0.05f;
          if (key_event.mKey == EKey::A)
            mSegmentRotation *= AngleAxis(delta_angle, Up<Vec3f>());
          if (key_event.mKey == EKey::D)
            mSegmentRotation *= AngleAxis(delta_angle, -Up<Vec3f>());
          if (key_event.mKey == EKey::S)
            mSegmentRotation *= AngleAxis(delta_angle, Forward<Vec3f>());
          if (key_event.mKey == EKey::W)
            mSegmentRotation *= AngleAxis(delta_angle, -Forward<Vec3f>());
        }
        else if (key_event.IsAltModifierPressed())
        {
          const auto delta_length = 0.1f;
          if (key_event.mKey == EKey::W)
            mSegmentLength += delta_length;
          if (key_event.mKey == EKey::S)
            mSegmentLength = std::max(mSegmentLength - delta_length, MinLength);
        }
        else
        {
          const auto delta_displacement = 0.1f;
          if (key_event.mKey == EKey::A)
            mSegmentTranslation += delta_displacement * Left<Vec3f>();
          if (key_event.mKey == EKey::D)
            mSegmentTranslation += delta_displacement * Right<Vec3f>();
          if (key_event.mKey == EKey::E)
            mSegmentTranslation += delta_displacement * Down<Vec3f>();
          if (key_event.mKey == EKey::Q)
            mSegmentTranslation += delta_displacement * Up<Vec3f>();
          if (key_event.mKey == EKey::W)
            mSegmentTranslation += delta_displacement * Forward<Vec3f>();
          if (key_event.mKey == EKey::S)
            mSegmentTranslation += delta_displacement * Back<Vec3f>();
        }
      }
    }
  };

  std::array<Segment3f, 32> GetSegments() const
  {
    std::array<Segment3f, 32> segments;
    for (int i = 0; i < segments.size(); ++i)
    {
      const auto f = (float(i) / segments.size());
      const auto segment_local_rotation = AngleAxis(f * FullCircleRads<float>(), Right<Vec3f>());
      const auto segment_rotation = (mSegmentRotation * segment_local_rotation);
      segments[i] = Segment3f { mSegmentTranslation + segment_rotation * (MinLength * Forward<Vec3f>()),
        mSegmentTranslation + segment_rotation * (mSegmentLength * Forward<Vec3f>()) };
    }
    return segments;
  }

  bool IsControlEnabled() const { return mControlEnabled; }

private:
  bool mControlEnabled = false;

  Quatf mSegmentRotation = AngleAxis(QuarterCircleRads<float>(), Up<Vec3f>());
  Vec3f mSegmentTranslation = Zero<Vec3f>();
  float mSegmentLength = 1.0f;
};

int main(int argc, const char** argv)
{
  srand(1234);

  const auto aabox = MakeAAHyperBoxFromCenterSize(Vec3f { -1.5f, 0.0f, 0.0f }, Vec3f { 1.0f, 1.5f, 1.8f });
  const auto sphere = Spheref { Vec3f { 1.5f, -0.2f, 0.1f }, 0.7f };
  const auto aacube = AACubef { Vec3f { 0.0f, 0.4f, -0.1f }, 0.6f };
  const auto cylinder = Cylinderf { Vec3f { -2.0f, -0.9f, -0.9f }, Vec3f { 2.3f, -2.4f, 0.3f }, 0.4f };
  const auto capsule = Capsulef { Vec3f { 0.0f, -0.9f, 0.9f }, Vec3f { 0.3f, -2.4f, -1.0f }, 0.3f };
  const auto primitives = std::make_tuple(aabox, sphere, aacube, cylinder, capsule);

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Segment AABox intersection";
  const auto window = std::make_shared<Window>(window_create_options);

  TestSegmentController segment_controller;
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
      renderer.DrawSegment(segment);

      ForEach(primitives, [&](const auto& in_primitive) {
        const auto intersection_distances = IntersectAll(segment, in_primitive);
        for (const auto& intersection_distance : intersection_distances)
        {
          if (!intersection_distance)
            continue;

          const auto intersection_point = (segment.GetFromPoint() + Direction(segment) * (*intersection_distance));
          renderer.SetPointSize(8.0f);
          renderer.SetDepthFunc(GL::EDepthFunc::ALWAYS);
          renderer.DrawPoint(intersection_point);
        }
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