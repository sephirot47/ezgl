#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "ShaderProgram.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Vec.h"
#include "VAOVertexAttrib.h"

using namespace egl;

static std::string GetFileContents(const std::string& filepath)
{
    std::ifstream ifs(filepath.c_str());
    const std::string contents((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    return contents;
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode,
    int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void render(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
}

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        throw std::runtime_error("Error initiating GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_RESIZABLE, true);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Error creating GLFW window");
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    const auto glew_init_error = glewInit();
    if (glew_init_error != GLEW_OK)
    {
        throw std::runtime_error("Error initiating GLEW");
    }

    glfwSetKeyCallback(window, key_callback);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLfloat vertices[] = {
        -0.5f, 0.5f, // Top-left
        0.5f, 0.5f, // Top-right
        0.5f, -0.5f, // Bottom-right
        -0.5f, -0.5f // Bottom-left
    };
    const VBO verticesVBO(vertices, sizeof(vertices));

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, // Top-left
        0.0f, 1.0f, 0.0f, // Top-right
        0.0f, 0.0f, 1.0f, // Bottom-right
        1.0f, 1.0f, 1.0f // Bottom-left
    };
    const VBO colorsVBO(colors, sizeof(colors));

    // Create an element array
    GLuint elements[]
        = {
            0, 1, 2,
            2, 3, 0
          };
    const EBO ebo(elements, sizeof(elements));

    const VertexShader vertexShader(GetFileContents("../res/default.vert"));
    const FragmentShader fragmentShader(GetFileContents("../res/default.frag"));
    const ShaderProgram shaderProgram(vertexShader, fragmentShader);
    shaderProgram.Use();

    // Specify the layout of the vertex data
    const GLint posAttrib = shaderProgram.GetAttribLocation("position");
    const GLint colorAttrib = shaderProgram.GetAttribLocation("color");

    VAO vao;
    vao.AddVBO(verticesVBO, posAttrib, VAOVertexAttribT<Vec2<float> >());
    vao.AddVBO(colorsVBO, colorAttrib, VAOVertexAttribT<Vec3<float> >());
    vao.AddVBO(verticesVBO, posAttrib, VAOVertexAttrib(2, EGLType::FLOAT, false, 2 * sizeof(float), 0));
    vao.AddVBO(colorsVBO, colorAttrib, VAOVertexAttrib(3, EGLType::FLOAT, false, 3 * sizeof(float), 0));
    vao.SetEBO(ebo);

    // Vec4<int> v(Vec4<int>::One());
    const auto v = Vec<float, 99>::One();
    // v[0] = 2;
    // v[1] = 3;
    // v[2] = 4;
    // v[3] = 73272398;
    std::cout << v << std::endl;
    // std::cout << v << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glfwWaitEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}