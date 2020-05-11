#include "ez/CameraControllerFly.h"
#include "ez/MeshFactory.h"
#include "ez/Octree.h"
#include "ez/PerspectiveCamera.h"
#include "ez/Plane.h"
#include "ez/Renderer3D.h"
#include "ez/VAO.h"
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
  const auto mesh = MeshFactory::GetCone(32);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  // camera->SetPosition(Left<Vec3f>() * 3.0f + Forward<Vec3f>() * 0.75f);
  // camera->LookAtPoint(Forward<Vec3f>() * 0.75f);

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  // Create octree
  const auto octrees = std::array {
    Octree<Triangle3f>(MakeSpan(mesh.GetTriangles()), 0, 1),
    Octree<Triangle3f>(MakeSpan(mesh.GetTriangles()), 0, 2),
    Octree<Triangle3f>(MakeSpan(mesh.GetTriangles()), 0, 3),
    Octree<Triangle3f>(MakeSpan(mesh.GetTriangles()), 0, 4),
    // Octree<Triangle3f>(MakeSpan(mesh.GetMeshTriangles()), 0, 5),
  };

  // Create renderer
  Renderer3D renderer;

  std::vector<Ray3f> rays;
  std::vector<Triangle3f> hit_triangles;
  window->SetInputEventCallback([&](const InputEvent& inInputEvent) {
    if (inInputEvent.GetType() == InputEvent::EType::MOUSE_BUTTON)
    {
      const auto& mouse_button_event = inInputEvent.As<InputEvent::EType::MOUSE_BUTTON>();
      if (mouse_button_event.IsPress() && mouse_button_event.mButton == EMouseButton::LEFT)
      {
        const auto mouse_position = window->GetMousePositionViewport();
        const auto mouse_ray = camera->GetViewportRay(mouse_position);
        const auto mesh_triangles = mesh.GetTriangles();
        for (const auto& mesh_triangle : mesh_triangles)
        {
          if (const auto intersection_point = Intersect(mouse_ray, mesh_triangle))
          {
            hit_triangles.push_back(mesh_triangle);
          }
        }
        rays.push_back(mouse_ray);
      }
    }
  });

  // Window loop
  int selected_octree = 0;
  window->Loop([&](const DeltaTime& inDeltaTime) {
    if (window->IsKeyPressed(EKey::UP))
    {
      selected_octree = (selected_octree + 1) % octrees.size();
    }
    const auto& octree = octrees[selected_octree];

    // Prepare renderer
    renderer.ResetState();
    renderer.Clear();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);

    renderer.GetMaterial().SetLightingEnabled(false);
    renderer.GetMaterial().SetDiffuseColor(White<Color4f>());
    renderer.SetCullFaceEnabled(false);
    const auto plane = Planef(NormalizedSafe(Vec3f(0.0f, 1.0f, 1.0f)), Zero<Vec3f>());
    renderer.DrawPlane(plane);

    renderer.GetMaterial().SetDiffuseColor(Green<Color4f>());

    renderer.SetLineWidth(1.0f);
    for (const auto& ray : rays) renderer.DrawRay(ray);

    for (const auto& hit_triangle : hit_triangles)
    {
      renderer.SetLineWidth(5.0f);
      renderer.DrawTriangleBoundary(hit_triangle);
    }

    /*
    renderer.GetMaterial().SetDiffuseColor(Red<Color4f>());
    std::stack<const Octree<Triangle3f>*> octrees_stack;
    octrees_stack.push(&octree);
    while (!octrees_stack.empty())
    {
      const auto& child_octree = *(octrees_stack.top());
      octrees_stack.pop();

      if (child_octree.IsLeaf())
        renderer.DrawAACubeBoundary(child_octree.GetAACube());

      for (const auto& grandchild_octree : child_octree) { octrees_stack.push(&grandchild_octree); }
    }
    */

    // Add a light
    renderer.AddDirectionalLight(Down<Vec3f>(), White<Color3f>());

    // Draw a sphere
    renderer.ResetTransformMatrix();
    renderer.GetMaterial().SetLightingEnabled(true);
    renderer.GetMaterial().SetDiffuseColor(Cyan<Color4f>());
    renderer.DrawMesh(mesh); // Draw sphere

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    camera_controller_fly.Update(inDeltaTime);

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });

  return EXIT_SUCCESS;
}