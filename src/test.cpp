#include "Camera.h"
#include "CameraControllerFly.h"
#include "DrawableMesh.h"
#include "EBO.h"
#include "FileUtils.h"
#include "Image2D.h"
#include "Macros.h"
#include "Math.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "Renderer.h"
#include "Segment.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Span.h"
#include "StreamOperators.h"
#include "Texture.h"
#include "Texture2D.h"
#include "VAO.h"
#include "VAOVertexAttrib.h"
#include "VBO.h"
#include "VariadicRepeat.h"
#include "Window.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace egl;

int main()
{
  srand(time(0));

  const auto window = std::make_shared<Window>();

  window->SetInputEventCallback([](const InputEvent& inInputEvent) {
    switch (inInputEvent.GetType())
    {

    case InputEvent::Type::Key:
    {
      // const auto& key_event = inInputEvent.As<InputEvent::Type::Key>();
      // PEEK(int(key_event.mKey));
    }
    break;

    case InputEvent::Type::MouseButton:
    {
      // const auto mouse_button_event = inInputEvent.As<InputEvent::Type::MouseButton>();
      // PEEK(int(mouse_button_event.mButton));
    }
    break;

    case InputEvent::Type::MouseMove:
    {
      // const auto mouse_move_event = inInputEvent.As<InputEvent::Type::MouseMove>();
      // PEEK(mouse_move_event.mPosition);
    }
    break;

    case InputEvent::Type::MouseEnterExit:
    {
      // const auto mouse_enter_exit_event = inInputEvent.As<InputEvent::Type::MouseEnterExit>();
      // PEEK(mouse_enter_exit_event.mEntered);
    }
    break;

    case InputEvent::Type::MouseScroll:
    {
      // const auto mouse_scroll_event = inInputEvent.As<InputEvent::Type::MouseScroll>();
      // PEEK(mouse_scroll_event.mDeltaScroll);
    }
    break;
    }
  });

  DrawableMesh test_mesh;
  // test_mesh.Read("monkey.obj");
  // test_mesh = DrawableMeshFactory::GetCone(62);
  test_mesh = DrawableMeshFactory::GetTorus(20, 20, 0.5f);
  // test_mesh = DrawableMeshFactory::GetCube();
  // test_mesh = DrawableMeshFactory::GetCylinder(32);
  // test_mesh = DrawableMeshFactory::GetHemisphere(20, 20);
  // test_mesh = DrawableMeshFactory::GetSphere(20, 20);
  // test_mesh.Write("/home/sephirot47/ezgl/build/test_mesh.obj");
  test_mesh.Write("/home/sephirot47/ezgl/build/test_mesh.ply", false);
  test_mesh.Write("/home/sephirot47/ezgl/build/test_mesh.obj", false);
  // test_mesh.Read("/home/sephirot47/ezgl/build/untitled.ply");
  // PEEK(test_mesh.GetNeighborFacesIds(33));
  // PEEK(test_mesh.GetVertexIdFromCornerId(test_mesh.GetOppositeCornerId(test_mesh.GetCornerIdFromFaceIdAndVertexId(3, 4))));
  // exit(0);

  Image2D<Color4f> image;
  // image.Read("/home/sephirot47/Downloads/checkerboard.jpg");
  // image.Read("/home/sephirot47/Downloads/bricks.png");
  // image.Read("/home/sephirot47/Downloads/rainbow.jpeg");
  image.Read("/home/sephirot47/Downloads/bricks2.jpg");

  const auto texture = std::make_shared<Texture2D>(image);

  Renderer renderer;

  Vec3f obj_pos = Zero<Vec3f>();
  const auto camera = std::make_shared<Camera>();
  {
    camera->SetPosition(Vec3f(10, 10, -10) * 0.8f); // Random(All<Vec3f>(0.0f), All<Vec3f>(10.0f)));
    camera->LookAtPoint(obj_pos);

    PerspectiveParameters perspective_params;
    perspective_params.mZFar = 1000.0f;
    camera->SetPerspectiveParameters(perspective_params);
  }
  renderer.SetCamera(camera);

  CameraControllerFly camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  float time = 0.0f;
  while (!window->ShouldClose())
  {
    renderer.PushState();

    // camera->SetPosition(RandomSign<Vec3f>() * Random(All<Vec3f>(3.0f), All<Vec3f>(10.0f)));
    // camera->LookAtPoint(obj_pos);

    renderer.ClearBackground(Pink());
    renderer.ClearDepth();

    renderer.Translate(obj_pos);
    const auto q = AngleAxis(time * 0.4f, Normalized(Vec3f { 9.0f, 92.2f, -9.5f }));
    // renderer.Rotate(q);

    renderer.Scale(All<Vec3f>(10.0f));
    renderer.SetLineWidth(3.0f);
    renderer.DrawAxes();

    // renderer.Rotate(q);
    renderer.SetTexture(texture);
    renderer.Scale(All<Vec3f>(0.5f));
    renderer.SetColor(White());
    // renderer.SetColor(Blue());
    renderer.SetLightSpecularExponent(120.0f);
    renderer.DrawMesh(test_mesh);

    renderer.SetColor(Blue());
    renderer.DrawMesh(test_mesh, Renderer::EDrawType::WIREFRAME);

    renderer.SetPointSize(1.0f);
    renderer.SetColor(Red());
    renderer.DrawMesh(test_mesh, Renderer::EDrawType::POINTS);

    window->SwapBuffers();
    window->PollEvents();

    const auto delta_time = 30ms;
    camera_controller_fly.Update(delta_time);

    time += 0.03;
    std::this_thread::sleep_for(delta_time);

    renderer.PopState();
  }

  return EXIT_SUCCESS;
}