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

  const auto test_mesh = DrawableMeshFactory::GetTorus(20, 20, 0.5f);
  const auto image = Image2D<Color4f> { "/home/sephirot47/Downloads/bricks2.jpg" };
  const auto texture = std::make_shared<Texture2D>(image);

  const auto camera = std::make_shared<Camera>();
  camera->SetPosition(Vec3f(10, 10, 10) * 0.8f); // Random(All<Vec3f>(0.0f), All<Vec3f>(10.0f)));
  camera->LookAtPoint(Zero<Vec3f>());

  CameraControllerFly camera_controller_fly;
  camera_controller_fly.SetCamera(camera);
  camera_controller_fly.SetWindow(window);

  Renderer renderer;
  renderer.SetCamera(camera);
  auto time = TimeDuration { 0 };
  window->Loop([&](const DeltaTime& inDeltaTime) {
    renderer.PushState();

    time += inDeltaTime;

    renderer.ClearBackground(Pink());
    renderer.ClearDepth();

    GL::Viewport(Zero<Vec2i>(), window->GetFramebufferSize());
    camera->GetPerspectiveParameters().mAspectRatio = window->GetFramebufferAspectRatio();

    const auto q = AngleAxis(time.count() * (0.5f * time.count()), Normalized(Vec3f { 0.0f, 0.0f, 1.0f }));
    // renderer.Rotate(q);
    renderer.Scale(All<Vec3f>(10.0f));
    renderer.SetLineWidth(3.0f);
    renderer.DrawAxes();

    const auto obj_pos = Vec3f { 1.0f, 1.0f, 1.0f } * 0.0f;
    renderer.Translate(obj_pos);
    renderer.Rotate(q);
    renderer.SetTexture(nullptr);
    renderer.Scale(All<Vec3f>(0.5f));
    renderer.SetColor(White());
    renderer.SetLightSpecularExponent(120.0f);
    renderer.DrawMesh(test_mesh);

    renderer.SetColor(Blue());
    renderer.DrawMesh(test_mesh, Renderer::EDrawType::WIREFRAME);

    renderer.SetPointSize(1.0f);
    renderer.SetColor(Red());
    renderer.DrawMesh(test_mesh, Renderer::EDrawType::POINTS);

    camera_controller_fly.Update(inDeltaTime);

    renderer.PopState();
  });
  return EXIT_SUCCESS;
}