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
  window_create_options.mTitle = "Test Ray Intersection";
  const auto window = std::make_shared<Window>(window_create_options);

  // Create meshes
  const auto mesh = MeshFactory::GetTorus(30, 30, 0.5f);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(2.0f * Left<Vec3f>() + 2.0f * Forward<Vec3f>());
  camera->LookAtPoint(Zero<Vec3f>());

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  // Create octree
  const auto octree = Octree<Triangle3f>(MakeSpan(mesh.GetTriangles()), 0, 6);

  // Create renderer
  Renderer3D renderer;

  AACubef cube(Vec3f(-1.0f, -2.0f, -1.5f), Vec3f(0.0f, 0.5f, 2.0f));

  std::vector<Ray3f> rays;
  std::vector<Triangle3f> hit_triangles;
  std::vector<Vec3f> hit_points;
  window->SetInputEventCallback([&](const InputEvent& inInputEvent) {
    if (inInputEvent.GetType() == InputEvent::EType::MOUSE_BUTTON)
    {
      const auto& mouse_button_event = inInputEvent.As<InputEvent::EType::MOUSE_BUTTON>();
      if (mouse_button_event.IsPress() && mouse_button_event.mButton == EMouseButton::LEFT)
      {
        const auto mouse_position = window->GetMousePositionViewport();
        const auto mouse_ray = camera->GetViewportRay(mouse_position);
        const auto mesh_triangles = mesh.GetTriangles();

        const auto time_before = Now();
        for (int i = 0; i < 1000; ++i)
        {
          const auto mesh_intersections = octree.IntersectAll(mouse_ray);
          for (const auto& mesh_intersection_distance : mesh_intersections)
          {
            if (i == 0)
              hit_points.push_back(mouse_ray.GetPoint(mesh_intersection_distance));
            // hit_triangles.push_back(mesh_triangle);
          }
        }
        PEEK(TimeDuration(Now() - time_before).count());

        /*
        const auto aacube_intersection_distances = Intersect(mouse_ray, cube);
        for (const auto& aacube_intersection_distance : aacube_intersection_distances)
        {
          if (aacube_intersection_distance)
            hit_points.push_back(mouse_ray.GetPoint(*aacube_intersection_distance));
        }
        */

        rays.push_back(mouse_ray);
      }
    }
  });

  // Window loop
  int selected_octree = 0;
  window->Loop([&](const DeltaTime& inDeltaTime) {
    if (window->IsKeyPressed(EKey::R))
    {
      hit_triangles.clear();
      hit_points.clear();
      rays.clear();
    }

    // Prepare renderer
    renderer.ResetState();
    renderer.Clear();
    renderer.SetCamera(camera);
    renderer.AdaptToWindow(*window);

    renderer.GetMaterial().SetLightingEnabled(false);
    renderer.GetMaterial().SetDiffuseColor(White<Color4f>());
    renderer.SetCullFaceEnabled(false);

    renderer.GetMaterial().SetDiffuseColor(Green<Color4f>());

    renderer.SetLineWidth(1.0f);
    for (const auto& ray : rays) renderer.DrawRay(ray);

    renderer.DrawAxes();

    for (const auto& hit_triangle : hit_triangles)
    {
      renderer.SetLineWidth(5.0f);
      renderer.DrawTriangleBoundary(hit_triangle);
    }

    renderer.GetMaterial().SetDiffuseColor(Purple<Color4f>());
    for (const auto& hit_point : hit_points)
    {
      renderer.SetPointSize(5.0f);
      renderer.DrawPoint(hit_point);
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
    renderer.DrawMesh(mesh);
    // renderer.DrawAACube(cube);

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    camera_controller_fly.Update(inDeltaTime);

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });

  return EXIT_SUCCESS;
}