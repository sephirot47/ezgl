#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Macros.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "EBO.h"
#include "Math.h"
#include "VAO.h"
#include "VAOVertexAttrib.h"
#include "VariadicRepeat.h"
#include "VBO.h"

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
    if (!glfwInit())
        THROW_EXCEPTION("Error initiating GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_RESIZABLE, true);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        THROW_EXCEPTION("Error creating GLFW window");
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    const auto glew_init_error = glewInit();
    if (glew_init_error != GLEW_OK)
        THROW_EXCEPTION("Error initiating GLEW");

    constexpr std::array vertices {
        Vec3f { -0.5f, 0.5f, 0.0f },
        Vec3f { 0.5f, 0.5f, 0.0f },
        Vec3f { 0.5f, -0.5f, 0.0f },
        Vec3f { -0.5f, -0.5f, 0.0f }
    };
    const VBO vertices_vbo { vertices.data(), sizeof(vertices) };

    // Create an element array
    constexpr std::array elements { 0, 1, 2, 2, 3, 0 };
    const EBO ebo(elements.data(), sizeof(elements));

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

    constexpr Vec3f v { 1.0f, 2.0f, 3.0f };
    constexpr auto w = Vec3f::One();
    constexpr auto result = Cross(v, w);
    std::cout << result << std::endl
              << std::endl;

    double time = 0;
    shader_program.SetUniform("Color", Vec3f(1.0f, 0.0f, 0.0f));
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const auto q = Quatf::AngleAxis(time * 3.14f, Normalized(Vec3f { 1.0f, 0.4f, 0.2f }));
        const auto model_matrix = RotationMat4(q);
        shader_program.SetUniform("Model", model_matrix);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        time += 0.03;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}