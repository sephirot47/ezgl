#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <chrono>
#include <thread>

#include "Macros.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "EBO.h"
#include "Math.h"
#include "Span.h"
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

    constexpr std::array vertices {
        Vec3f { -0.5f, 0.5f, 0.0f },
        Vec3f { 0.5f, 0.5f, 0.0f },
        Vec3f { 0.5f, -0.5f, 0.0f },
        Vec3f { -0.5f, -0.5f, 0.0f }
    };
    const VBO vertices_vbo { MakeSpan(vertices) };

    // Create an element array
    constexpr std::array<GL::Uint, 6> elements { 0, 1, 2, 2, 3, 0 };
    const EBO ebo(MakeSpan(elements));

    const VertexShader vertex_shader { GetFileContents("../res/default.vert") };
    const FragmentShader fragment_shader { GetFileContents("../res/default.frag") };
    ShaderProgram shader_program { vertex_shader, fragment_shader };
    shader_program.Bind();

    // Specify the layout of the vertex data
    const auto pos_attrib = shader_program.GetAttribLocation("position");

    VAO vao;
    vao.Bind();
    vao.AddVBO(vertices_vbo, *pos_attrib, VAOVertexAttribT<Vec3f>());
    vao.SetEBO(ebo);

    double time = 0;
    shader_program.SetUniform("Color", Color3f { 1.0f, 1.0f, 0.0f });
    while (!window.ShouldClose())
    {
        // Clear the screen to black
        GL::ClearColor(Color4f { 0.0f, 0.0f, 0.0f, 1.0f });
        GL::ClearBuffer(GL::EBufferBitFlags::COLOR | GL::EBufferBitFlags::DEPTH);

        const auto q = Quatf::AngleAxis(time * 3.14f, Normalized(Vec3f { 1.0f, 0.4f, 0.2f }));
        const auto model_matrix = RotationMat4(q);
        shader_program.SetUniform("Model", model_matrix);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.SwapBuffers();
        window.PollEvents();

        time += 0.03;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return EXIT_SUCCESS;
}