#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <chrono>
#include <thread>

#include "Camera.h"
#include "DrawableMesh.h"
#include "Macros.h"
#include "Mesh.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "EBO.h"
#include "Math.h"
#include "Span.h"
#include "StreamOperators.h"
#include "VAO.h"
#include "VAOVertexAttrib.h"
#include "VariadicRepeat.h"
#include "VBO.h"
#include "Window.h"

using namespace egl;

static std::string GetFileContents(const std::filesystem::path& filepath)
{
    std::ifstream ifs(filepath.c_str());
    const std::string contents((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    return contents;
}

int main()
{
    Window window;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    const VertexShader vertex_shader { GetFileContents("../res/default.vert") };
    const FragmentShader fragment_shader { GetFileContents("../res/default.frag") };
    ShaderProgram shader_program { vertex_shader, fragment_shader };
    shader_program.Bind();

    DrawableMesh test_mesh;
    test_mesh.Read("test.obj");
    test_mesh.ComputeNormals();
    test_mesh.UpdateVAOs();

        Vec3f obj_pos = RandomUnit<Vec3f>() * 10.0f;

    Camera camera;
    {
        camera.SetPosition(Vec3f(0.0f, 0.0f, 0.0f));
        camera.LookAtPoint(obj_pos, Vec3f(0, 1, 0));

        PerspectiveParameters perspective_params;
        perspective_params.mZFar = 1000.0f;
        camera.SetPerspectiveParameters(perspective_params);
    }

    float time = 0.0f;
    shader_program.SetUniform("UColor", Color3f { 1.0f, 1.0f, 1.0f });
    while (!window.ShouldClose())
    {
        PEEK(obj_pos);

        // Clear the screen to black
        GL::ClearColor(Color4f { 0.0f, 0.0f, 0.0f, 1.0f });
        GL::ClearBuffer(GL::EBufferBitFlags::COLOR | GL::EBufferBitFlags::DEPTH);

        const auto q = AngleAxis(time * 3.14f * 0.1f, Normalized(Vec3f { 1.0f, 0.2f, -0.5f }));
        const auto model_matrix = TranslationMat4(obj_pos) * RotationMat4(q);
        const auto view_matrix = camera.GetViewMatrix();
        const auto projection_matrix = camera.GetProjectionMatrix();
        shader_program.SetUniform("UModel", model_matrix);
        shader_program.SetUniform("UView", view_matrix);
        shader_program.SetUniform("UProjection", projection_matrix);
        test_mesh.Draw();

        window.SwapBuffers();
        window.PollEvents();

        time += 0.03;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return EXIT_SUCCESS;
}