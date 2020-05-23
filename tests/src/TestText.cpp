#include "ez/CameraControllerFly.h"
#include "ez/Font.h"
#include "ez/Image2D.h"
#include "ez/MeshFactory.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Renderer2D.h"
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

  // Create renderers
  Renderer2D renderer2D;
  Renderer3D renderer3D;

  // Window loop
  auto time = 0.0f;
  window->Loop([&](const DeltaTime& inDeltaTime) {
    GL::Viewport(Zero<Vec2i>(), window->GetFramebufferSize());

    // Prepare renderers
    renderer3D.ResetState();
    renderer3D.SetCamera(camera);
    renderer3D.AdaptToWindow(*window);
    renderer3D.Clear(Black<Color4f>());

    renderer2D.ResetState();
    renderer2D.AdaptToWindow(*window);
    renderer2D.Clear(Black<Color4f>());

    // Add a light
    renderer3D.AddDirectionalLight(NormalizedSafe(Vec3f(-1.2f, -1.0f, -1.4f)), White<Color3f>());

    constexpr auto long_text
        = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789=;,:.()[]{}\"-_?!+*/\\%&$#@|^<>'";

    time += inDeltaTime.count();
    renderer3D.Rotate(AngleAxis((FullCircleRads() * time * 0.02f), Up<Vec3f>()));
    constexpr auto NumIterations = 40;
    for (int i = 0; i < NumIterations; ++i)
    {
      renderer3D.Translate(Down<Vec3f>() * 0.15f);
      renderer3D.Rotate(AngleAxis((FullCircleRads() / NumIterations), Up<Vec3f>()));
      renderer3D.GetMaterial().SetDiffuseColor(
          HSVToRGB(Color4f(static_cast<float>(i) / NumIterations, 1.0f, 1.0f, 1.0f)));
      renderer3D.DrawText(long_text, font_big, 0.001f);
    }

    renderer3D.ResetTransformMatrix();
    renderer3D.Translate(Down<Vec3f>() * 8.0f);
    renderer3D.Scale(0.3f);

    constexpr auto h_alignment_names = std::array { "left", "center", "right" };
    constexpr auto v_alignment_names = std::array { "top", "center", "baseline", "bottom" };
    for (const auto h_alignment : { ETextHAlignment::LEFT, ETextHAlignment::CENTER, ETextHAlignment::RIGHT })
    {
      for (const auto v_alignment :
          { ETextVAlignment::TOP, ETextVAlignment::CENTER, ETextVAlignment::BASELINE, ETextVAlignment::BOTTOM })
      {
        renderer3D.GetMaterial().SetDiffuseColor(Red<Color4f>());
        renderer3D.DrawSegment(Segment3f { Down<Vec3f>(), Up<Vec3f>() });
        renderer3D.DrawSegment(Segment3f { Left<Vec3f>() * 10.0f, Right<Vec3f>() * 10.0f });

        auto text = std::string {};
        text += h_alignment_names[static_cast<int>(h_alignment)];
        text += "_";
        text += v_alignment_names[static_cast<int>(v_alignment)];

        renderer3D.GetMaterial().SetDiffuseColor(Blue<Color4f>());
        renderer3D.DrawText(text, font_small, 0.04f, h_alignment, v_alignment);
        renderer3D.Translate(Down<Vec3f>() * 4.0f);
      }
    }

    renderer3D.Translate(Down<Vec3f>() * 2.0f);
    renderer3D.DrawText("Billboard", font_big, 0.04f, ETextHAlignment::CENTER, ETextVAlignment::CENTER, true, false);
    renderer3D.Translate(Down<Vec3f>() * 6.0f);
    renderer3D
        .DrawText("ConstantScale", font_big, 0.004f, ETextHAlignment::CENTER, ETextVAlignment::CENTER, false, true);
    renderer3D.Translate(Down<Vec3f>() * 10.0f);
    renderer3D.DrawText("Billboard & ConstantScale",
        font_big,
        0.004f,
        ETextHAlignment::CENTER,
        ETextVAlignment::CENTER,
        true,
        true);

    renderer2D.Translate(Vec2f { 0.0f, static_cast<float>(window->GetFramebufferSize()[1]) });
    renderer2D.GetMaterial().SetColor(Red<Color4f>());
    renderer2D.DrawText("Text in 2D", font_big, 0.2f, ETextHAlignment::LEFT, ETextVAlignment::TOP);

    renderer2D.GetMaterial().SetColor(Red<Color4f>());
    renderer2D.Translate(Vec2f { static_cast<float>(window->GetFramebufferSize()[0]), 0.0f });
    renderer2D.DrawAARect(AARectf { Vec2f { -110.0f, -30.0f }, Vec2f { 0.0f, 0.0f } });
    renderer2D.GetMaterial().SetColor(White<Color4f>());
    renderer2D.DrawText("Text in 2D", font_big, 0.2f, ETextHAlignment::RIGHT, ETextVAlignment::TOP);

    // Blit image to window
    GL::ClearDepth();
    renderer2D.Blit();
    renderer3D.Blit();

    camera_controller_fly.Update(inDeltaTime);

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });
  return EXIT_SUCCESS;
}