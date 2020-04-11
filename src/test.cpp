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
  Window window;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  DrawableMesh test_mesh;
  test_mesh.Read("test.obj");
  test_mesh.UpdateVAOs(DrawableMesh::ENormalsType::FLAT);
  // test_mesh = DrawableMeshFactory::GetCube();

  Renderer renderer;

  Vec3f obj_pos = Vec3f(-5.0f, -5.0f, -5.0f);
  Camera camera;
  {
    camera.SetPosition(Vec3f(0.0f, 10.0f, 0.0f));
    camera.LookAtPoint(obj_pos, Vec3f(0.0f, 1.0f, 0.0f));

    PerspectiveParameters perspective_params;
    perspective_params.mZFar = 1000.0f;
    camera.SetPerspectiveParameters(perspective_params);
  }
  renderer.SetCamera(camera);

  float time = 0.0f;
  while (!window.ShouldClose())
  {
    // Clear the screen to black
    GL::ClearColor(Color4f { 0.0f, 0.0f, 0.0f, 1.0f });
    GL::ClearBuffer(GL::EBufferBitFlags::COLOR | GL::EBufferBitFlags::DEPTH);

    const auto q = AngleAxis(time * 3.14f * 0.1f, Normalized(Vec3f { 1.0f, 0.2f, -0.5f }));
    const auto model_matrix = TranslationMat4(obj_pos) * RotationMat4(q);
    renderer.SetModelMatrix(model_matrix);
    renderer.SetColor(Color4f { 1.0f, 1.0f, 1.0f, 1.0f });
    renderer.DrawMesh(test_mesh);

renderer.DrawAxes(5.0f);

    window.SwapBuffers();
    window.PollEvents();

    time += 0.03;
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  return EXIT_SUCCESS;
}