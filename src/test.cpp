#include "Camera.h"
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

  Window window;

  DrawableMesh test_mesh;
  test_mesh.Read("monkey.obj");
  // test_mesh = DrawableMeshFactory::GetCube();
  // test_mesh = DrawableMeshFactory::GetHemisphere(99, 99);
  // test_mesh = DrawableMeshFactory::GetSphere(99, 99);
  // test_mesh.Write("/home/sephirot47/ezgl/build/test_mesh.obj");

  Renderer renderer;

  // Vec3f obj_pos = Vec3f(-5.0f, -5.0f, -5.0f) * 0.3f;
  Vec3f obj_pos = Vec3f(0.0f, 0.0f, 0.0f);
  Camera camera;
  {
    camera.SetPosition(Vec3f(5.0f, 0.0f, 8.0f) * 1.5f);
    camera.LookAtPoint(obj_pos, Vec3f(0.0f, 1.0f, 0.0f));

    PerspectiveParameters perspective_params;
    perspective_params.mZFar = 1000.0f;
    camera.SetPerspectiveParameters(perspective_params);
  }
  renderer.SetCamera(camera);

  float time = 0.0f;
  while (!window.ShouldClose())
  {
    renderer.PushState();

    renderer.ClearBackground(Pink());
    renderer.ClearDepth();

    const auto q = AngleAxis(time * 0.4f, Normalized(Vec3f { 9.0f, 92.2f, -9.5f }));
    renderer.Translate(obj_pos);
    renderer.Rotate(q);

    renderer.Scale(All<Vec3f>(10.0f));
    renderer.SetLineWidth(3.0f);
    renderer.DrawAxes();

    renderer.Rotate(q);
    renderer.Scale(All<Vec3f>(0.5f));
    renderer.SetColor(Red());
    renderer.SetLightSpecularExponent(60.0f);
    renderer.DrawMesh(test_mesh);

    renderer.SetColor(Blue());
    // renderer.DrawMesh(test_mesh, Renderer::EDrawType::WIREFRAME);

    renderer.SetPointSize(4.0f);
    renderer.SetColor(Red());
    // renderer.DrawMesh(test_mesh, Renderer::EDrawType::POINTS);

    window.SwapBuffers();
    window.PollEvents();

    time += 0.03;
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    renderer.PopState();
  }

  return EXIT_SUCCESS;
}