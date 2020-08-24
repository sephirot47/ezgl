#include "ez/CameraControllerFly.h"
#include "ez/HyperSphere.h"
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
  // Create meshes
  const auto mesh = MeshFactory::GetTorus(30, 30, 0.5f);

  // Create octree
  const auto octree = Octree<Triangle3f>(MakeSpan(mesh.GetTriangles()), 8, 6);

  // Create window
  Window::CreateOptions window_create_options;
  window_create_options.mTitle = "Test Ray Intersection";
  const auto window = std::make_shared<Window>(window_create_options);

  // Camera
  const auto camera = std::make_shared<PerspectiveCameraf>();
  camera->SetPosition(5.0f * Forward<Vec3f>());
  camera->LookAtPoint(Zero<Vec3f>());

  // Camera controller
  CameraControllerFly3f camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  // Create renderer
  Renderer3D renderer;

  const auto box_position = 3.0f * Right<Vec3f>();
  AABoxf box(box_position + Vec3f(-1.0f, -1.0f, -1.0f), box_position + Vec3f(0.0f, 0.6f, 1.2f));

  Spheref sphere;
  sphere.SetRadius(1.3f);
  sphere.SetCenter(6.0f * Right<Vec3f>());

  std::vector<Ray3f> rays;
  std::vector<Triangle3f> hit_triangles;
  std::vector<Vec3f> hit_points;
  window->SetMouseButtonEventCallback([&](const MoustButtonEvent& inMoustButtonEvent) {
    if (inMoustButtonEvent.IsPress() && inMoustButtonEvent.mButton == EMouseButton::LEFT)
    {
      const auto mouse_position = window->GetMousePositionViewport();
      const auto mouse_ray = camera->GetViewportRay(mouse_position);

      rays.push_back(mouse_ray);

      const auto mesh_triangles = mesh.GetTriangles();
      const auto mesh_intersections = IntersectAll(octree, mouse_ray);
      for (const auto& mesh_intersection : mesh_intersections)
      {
        hit_points.push_back(mouse_ray.GetPoint(mesh_intersection.mDistance));
        hit_triangles.push_back(mesh_triangles.at(mesh_intersection.mPrimitiveIndex));
      }

      const auto sphere_intersection_result = IntersectAll(mouse_ray, sphere);
      for (const auto& sphere_intersection : sphere_intersection_result)
      {
        if (sphere_intersection)
          hit_points.push_back(mouse_ray.GetPoint(*sphere_intersection));
      }

      const auto box_intersection_result = IntersectAll(mouse_ray, box);
      for (const auto& box_intersection : box_intersection_result)
      {
        if (box_intersection)
          hit_points.push_back(mouse_ray.GetPoint(*box_intersection));
      }
    }
  });

  // Window loop
  window->Loop([&](const DeltaTime& inDeltaTime) {
    if (window->IsKeyPressed(EKey::R))
    {
      hit_triangles.clear();
      hit_points.clear();
      rays.clear();
    }

    GL::Viewport(Zero<Vec2i>(), window->GetFramebufferSize());

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
        renderer.DrawAABoxBoundary(child_octree.GetAABox());

      for (const auto& grandchild_octree : child_octree) { octrees_stack.push(&grandchild_octree); }
    }
    */

    // Add a light
    renderer.AddDirectionalLight(Normalized(Vec3f(-1.0f, -1.5f, 2.0f)), White<Color3f>());

    renderer.ResetTransformMatrix();
    renderer.GetMaterial().SetLightingEnabled(true);
    renderer.GetMaterial().SetDiffuseColor(Cyan<Color4f>());

    renderer.DrawMesh(mesh);
    renderer.DrawAABox(box);
    renderer.DrawSphere(sphere);

    // Blit image to window
    GL::ClearDepth();
    renderer.Blit();

    camera_controller_fly.Update(inDeltaTime);

    // Keep looping
    return Window::ELoopResult::KEEP_LOOPING;
  });

  return EXIT_SUCCESS;
}