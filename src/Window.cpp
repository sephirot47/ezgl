#include "Window.h"
#include "GL.h"
#include "Macros.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace egl
{
// During init, enable debug output
Window::Window()
{
  if (!glfwInit())
    THROW_EXCEPTION("Error initiating GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);
  glfwWindowHint(GLFW_RESIZABLE, true);

  mHandle = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
  if (!mHandle)
    THROW_EXCEPTION("Error creating GLFW window");

  glfwMakeContextCurrent(mHandle);

  glewExperimental = GL_TRUE;
  const auto glew_init_error = glewInit();
  if (glew_init_error != GLEW_OK)
    THROW_EXCEPTION("Error initiating GLEW");

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(&GL::ErrorCallback, 0);
}

Window::~Window()
{
  glfwDestroyWindow(mHandle);
  glfwTerminate();
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(mHandle); }

void Window::SwapBuffers() { glfwSwapBuffers(mHandle); }

void Window::PollEvents() { glfwPollEvents(); }

}