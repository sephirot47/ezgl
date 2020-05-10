#include "ez/CameraControllerFly.h"
#include "ez/MeshFactory.h"
#include "ez/Octree.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Renderer3D.h"
#include "ez/Window.h"
#include <cstdlib>
#include <stack>
#include <vector>

using namespace ez;

int main(int argc, const char** argv)
{
  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Octrees";
  const auto window = std::make_shared<Window>(window_create_options);

  // Create meshes
  const auto torus_mesh = MeshFactory::GetTorus(32, 32, 0.5f);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(Back<Vec3f>() * 5.0f);
  camera->LookAtPoint(Zero<Vec3f>());

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  // Create octree
  const Octree<Triangle3f> octree(MakeSpan(torus_mesh.GetMeshTriangles()));
  PEEK(octree.GetAACube());

  // Create renderer
  Renderer3D renderer;

  // Window loop
  window->Loop([&](const DeltaTime& inDeltaTime) {
    // Prepare renderer
    renderer.ResetState();
    renderer.Clear();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);

    renderer.GetMaterial().SetDiffuseColor(Red<Color4f>());
    std::stack<const Octree<Triangle3f>*> octrees_stack;
    octrees_stack.push(&octree);
    while (!octrees_stack.empty())
    {
      const auto& child_octree = *(octrees_stack.top());
      octrees_stack.pop();
      renderer.DrawAACubeBoundary(child_octree.GetAACube());

      for (const auto& grandchild_octree : child_octree) { octrees_stack.push(&grandchild_octree); }
    }

    // Add a light
    renderer.AddDirectionalLight(Down<Vec3f>(), White<Color3f>());

    // Draw a sphere
    renderer.GetMaterial().SetDiffuseColor(Cyan<Color4f>());
    renderer.DrawMesh(torus_mesh); // Draw sphere

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    camera_controller_fly.Update(inDeltaTime);

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });

  return EXIT_SUCCESS;
}