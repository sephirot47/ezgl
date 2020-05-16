#include "ez/PerspectiveCamera.h"
#include "ez/Renderer3D.h"
#include "ez/Window.h"
#include <cstdlib>

using namespace ez;

int main(int argc, const char** argv)
{
  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Hello EzWorld";
  Window window(window_create_options);

  // Create renderer
  Renderer3D renderer;

  // Window loop
  window.Loop([&](const DeltaTime& inDeltaTime) {

    GL::Viewport(Zero<Vec2i>(), window.GetFramebufferSize());

    // Prepare renderer
    renderer.ResetState();
    renderer.AdaptToWindow(window);

    // Set our camera
    renderer.GetCamera()->SetPosition(Back<Vec3f>() * 5.0f);
    renderer.GetCamera()->LookAtPoint(Zero<Vec3f>());

    // Add a light
    renderer.AddDirectionalLight(Down<Vec3f>(), Yellow<Color3f>());

    // Draw a sphere
    renderer.GetMaterial().SetDiffuseColor(Red<Color4f>()); // Set red color
    renderer.Scale(Vec3f(2.0f, 2.0f, 2.0f));                // Scale
    renderer.DrawSphere();                                  // Draw sphere

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });

  return EXIT_SUCCESS;
}