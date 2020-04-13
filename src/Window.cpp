#include "Window.h"
#include "GL.h"
#include "Macros.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace egl
{
// During init, enable debug output
Window::Window(const Window::CreateOptions& inCreateOptions)
{
  if (!glfwInit())
    THROW_EXCEPTION("Error initiating GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, inCreateOptions.mGLVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, inCreateOptions.mGLVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);

  if (inCreateOptions.mResizable)
    glfwWindowHint(GLFW_RESIZABLE, true);

  if (inCreateOptions.mUseAntialiasing)
    glfwWindowHint(GLFW_SAMPLES, 4);

  mHandle = glfwCreateWindow(inCreateOptions.mWidth,
      inCreateOptions.mHeight,
      inCreateOptions.mTitle.data(),
      nullptr,
      nullptr);
  if (!mHandle)
    THROW_EXCEPTION("Error creating GLFW window");

  glfwMakeContextCurrent(mHandle);

  glewExperimental = GL_TRUE;
  const auto glew_init_error = glewInit();
  if (glew_init_error != GLEW_OK)
    THROW_EXCEPTION("Error initiating GLEW");

  if (inCreateOptions.mUseAntialiasing)
  {
    GL::Enable(GL::Enablable::LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  }

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