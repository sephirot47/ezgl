#include "Camera.h"
#include "CameraControllerFly.h"
#include "DrawableMesh.h"
#include "EBO.h"
#include "FileUtils.h"
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
  test_mesh.Read("monkey.obj");
  // test_mesh = DrawableMeshFactory::GetCube();
  test_mesh = DrawableMeshFactory::GetHemisphere(99, 99);
  // test_mesh = DrawableMeshFactory::GetSphere(99, 99);
  // test_mesh.Write("/home/sephirot47/ezgl/build/test_mesh.obj");

  Renderer renderer;

  Vec3f obj_pos = Vec3f(0.0f, 0.0f, 0.0f);
  const auto camera = std::make_shared<Camera>();
  {
    camera->SetPosition(Vec3f(5.0f, 0.0f, 8.0f) * 1.0f);
    camera->LookAtPoint(obj_pos, Up<Vec3f>());

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

    renderer.ClearBackground(Pink());
    renderer.ClearDepth();

    camera_controller_fly.Update(30ms);

    renderer.Translate(obj_pos);
    // const auto q = AngleAxis(time * 0.4f, Normalized(Vec3f { 9.0f, 92.2f, -9.5f }));
    // renderer.Rotate(q);

    renderer.Scale(All<Vec3f>(10.0f));
    renderer.SetLineWidth(3.0f);
    renderer.DrawAxes();

    // renderer.Rotate(q);
    renderer.Scale(All<Vec3f>(0.5f));
    renderer.SetColor(Red());
    renderer.SetLightSpecularExponent(120.0f);
    renderer.DrawMesh(test_mesh);

    renderer.SetColor(Blue());
    // renderer.DrawMesh(test_mesh, Renderer::EDrawType::WIREFRAME);

    renderer.SetPointSize(4.0f);
    renderer.SetColor(Red());
    // renderer.DrawMesh(test_mesh, Renderer::EDrawType::POINTS);

    window->SwapBuffers();
    window->PollEvents();

    time += 0.03;
    std::this_thread::sleep_for(30ms);

    renderer.PopState();
  }

  return EXIT_SUCCESS;
}