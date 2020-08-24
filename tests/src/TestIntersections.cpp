#include <ez/AAHyperBox.h>
#include <ez/BinaryIndex.h>
#include <ez/CameraControllerFly.h>
#include <ez/Macros.h>
#include <ez/OrthographicCamera.h>
#include <ez/PerspectiveCamera.h>
#include "ez/Renderer3D.h"
#include <ez/Triangle.h>
#include <ez/Window.h>
#include <cstdlib>

using namespace ez;
using namespace std::placeholders;

int main(int argc, const char** argv)
{
  srand(1234);

  constexpr auto NumTriangles = 20;
  constexpr auto NumAABoxs = 30;
  constexpr auto SpaceSize = 5.0f;
  constexpr auto TriSize = 3.0f;

  std::array<AABoxf, NumAABoxs> aa_boxes;
  for (auto& aa_box : aa_boxes)
  {
    aa_box = AABoxf(Random(All<Vec3f>(-1.0f), All<Vec3f>(0.0f)), Random(All<Vec3f>(0.0f), All<Vec3f>(1.0f)));
    aa_box = Transformed(aa_box, TranslationMat(Random(All<Vec3f>(-SpaceSize), All<Vec3f>(SpaceSize))));
  }

  std::array<Triangle3f, NumTriangles> triangles;
  for (auto& triangle : triangles)
  {
    triangle = Random(Triangle3f(All<Vec3f>(-TriSize), All<Vec3f>(-TriSize), All<Vec3f>(-TriSize)),
        Triangle3f(All<Vec3f>(TriSize), All<Vec3f>(TriSize), All<Vec3f>(TriSize)));
    triangle = Transformed(triangle, TranslationMat(Random(All<Vec3f>(-SpaceSize), All<Vec3f>(SpaceSize))));
  }

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test intersections";
  const auto window = std::make_shared<Window>(window_create_options);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(Back<Vec3f>() * 5.0f);
  camera->LookAtPoint(Zero<Vec3f>());

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  // Create renderer
  Renderer3D renderer;

  // Window loop
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    renderer.Clear();

    renderer.GetMaterial().SetLightingEnabled(false);

    camera_controller_fly.Update(inDeltaTime);

    const auto Intersects = [](const auto& lhs, const auto& rhs) {
      return (reinterpret_cast<const void*>(&lhs) != reinterpret_cast<const void*>(&rhs)) && IntersectCheck(lhs, rhs);
    };

    const auto IntersectsWithSomething = [&](const auto& inObject) {
      return std::any_of(triangles.begin(), triangles.end(), [&](const auto& inOther) {
        return Intersects(inObject, inOther);
      }) || std::any_of(aa_boxes.begin(), aa_boxes.end(), [&](const auto& inOther) {
        return Intersects(inObject, inOther);
      });
    };

    renderer.SetLineWidth(2.0f);
    renderer.SetCullFaceEnabled(false);
    for (const auto& triangle : triangles)
    {
      const auto intersect = IntersectsWithSomething(triangle);
      renderer.GetMaterial().SetDiffuseColor(WithAlpha(intersect ? Red<Color4f>() : Green<Color4f>(), 0.5f));
      renderer.DrawTriangle(triangle);
      renderer.GetMaterial().SetDiffuseColor(White<Color4f>());
      renderer.DrawTriangleBoundary(triangle);
    }

    renderer.SetCullFaceEnabled(true);
    for (const auto& aa_box : aa_boxes)
    {
      const auto intersect = IntersectsWithSomething(aa_box);
      renderer.GetMaterial().SetDiffuseColor(WithAlpha(intersect ? Red<Color4f>() : Green<Color4f>(), 0.5f));
      renderer.DrawAABox(aa_box);
      renderer.GetMaterial().SetDiffuseColor(White<Color4f>());
      renderer.DrawAABoxBoundary(aa_box);
    }

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });

  return EXIT_SUCCESS;
}