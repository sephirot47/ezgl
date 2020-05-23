#include "ez/CameraControllerFly.h"
#include "ez/Font.h"
#include "ez/Image2D.h"
#include "ez/MeshFactory.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Renderer3D.h"
#include "ez/Window.h"
#include <cstdlib>

using namespace ez;

int main(int argc, const char** argv)
{
  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Text";
  const auto window = std::make_shared<Window>(window_create_options);

  Font font_big("/home/sephirot47/ezgl/res/Fonts/Ubuntu-Regular.ttf", 128);
  Font font_small("/home/sephirot47/ezgl/res/Fonts/Ubuntu-Regular.ttf", 50);

  Mesh mesh = MeshFactory::GetTorus(10, 10, 0.5f);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(Back<Vec3f>() * 3.0f);
  camera->LookAtPoint(Zero<Vec3f>());

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  // Create renderer
  Renderer3D renderer;

  // Window loop
  auto time = 0.0f;
  window->Loop([&](const DeltaTime& inDeltaTime) {
    GL::Viewport(Zero<Vec2i>(), window->GetFramebufferSize());

    // Prepare renderer
    renderer.ResetState();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);
    renderer.Clear(Black<Color4f>());

    // Add a light
    renderer.AddDirectionalLight(NormalizedSafe(Vec3f(-1.2f, -1.0f, -1.4f)), White<Color3f>());

    constexpr auto long_text
        = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789=;,:.()[]{}\"-_?!+*/\\%&$#@|^<>'";

    time += inDeltaTime.count();
    renderer.Rotate(AngleAxis((FullCircleRads() * time * 0.02f), Up<Vec3f>()));
    renderer.Scale(0.001f);
    constexpr auto NumIterations = 30;
    for (int i = 0; i < NumIterations; ++i)
    {
      renderer.Translate(Down<Vec3f>() * 100.0f);
      renderer.Rotate(AngleAxis((FullCircleRads() / NumIterations), Up<Vec3f>()));
      renderer.GetMaterial().SetDiffuseColor(
          HSVToRGB(Color4f(static_cast<float>(i) / NumIterations, 1.0f, 1.0f, 1.0f)));
      renderer.DrawText(long_text, font_big);
    }

    renderer.ResetTransformMatrix();
    renderer.Translate(Down<Vec3f>() * 6.0f);
    renderer.Scale(0.3f);

    constexpr auto h_alignment_names = std::array { "left", "center", "right" };
    constexpr auto v_alignment_names = std::array { "top", "center", "baseline", "bottom" };
    for (const auto h_alignment : { ETextHAlignment::LEFT, ETextHAlignment::CENTER, ETextHAlignment::RIGHT })
    {
      for (const auto v_alignment :
          { ETextVAlignment::TOP, ETextVAlignment::CENTER, ETextVAlignment::BASELINE, ETextVAlignment::BOTTOM })
      {
        renderer.GetMaterial().SetDiffuseColor(Red<Color4f>());
        renderer.DrawSegment(Segment3f { Down<Vec3f>(), Up<Vec3f>() });
        renderer.DrawSegment(Segment3f { Left<Vec3f>() * 10.0f, Right<Vec3f>() * 10.0f });
        renderer.PushTransformMatrix();
        renderer.Scale(0.04f);

        auto text = std::string {};
        text += h_alignment_names[static_cast<int>(h_alignment)];
        text += "_";
        text += v_alignment_names[static_cast<int>(v_alignment)];

        renderer.GetMaterial().SetDiffuseColor(Blue<Color4f>());
        renderer.DrawText(text, font_small, h_alignment, v_alignment);
        renderer.PopTransformMatrix();

        renderer.Translate(Down<Vec3f>() * 4.0f);
      }
    }

    renderer.Translate(Down<Vec3f>() * 2.0f);
    renderer.PushTransformMatrix();
    renderer.Scale(0.04f);
    renderer.DrawText("Billboard", font_big, ETextHAlignment::CENTER, ETextVAlignment::CENTER, true, false);
    renderer.PopTransformMatrix();
    renderer.Translate(Down<Vec3f>() * 6.0f);

    renderer.PushTransformMatrix();
    renderer.Scale(0.004f);
    renderer.DrawText("ConstantScale", font_big, ETextHAlignment::CENTER, ETextVAlignment::CENTER, false, true);
    renderer.PopTransformMatrix();
    renderer.Translate(Down<Vec3f>() * 10.0f);

    renderer.PushTransformMatrix();
    renderer.Scale(0.004f);
    renderer
        .DrawText("Billboard & ConstantScale", font_big, ETextHAlignment::CENTER, ETextVAlignment::CENTER, true, true);
    renderer.PopTransformMatrix();

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    camera_controller_fly.Update(inDeltaTime);

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });
  return EXIT_SUCCESS;
}