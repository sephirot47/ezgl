#pragma once

#include <cstdint>
#include <string_view>

class GLFWwindow;

namespace egl
{
class Window
{
public:
  struct CreateOptions
  {
    CreateOptions() {};

    uint32_t mWidth = 800;
    uint32_t mHeight = 800;

    std::string_view mTitle = "EZGLWindow";

    uint32_t mGLVersionMajor = 4;
    uint32_t mGLVersionMinor = 0;

    bool mResizable = true;
    bool mUseAntialiasing = true;
  };


  Window(const Window::CreateOptions &inCreateOptions = Window::CreateOptions());
  Window(const Window& inRHS) = delete;
  Window& operator=(const Window& inRHS) = delete;
  Window(Window&& inRHS) = delete;
  Window& operator=(Window&& inRHS) = delete;
  ~Window();

  bool ShouldClose() const;
  void SwapBuffers();
  void PollEvents();

private:
  GLFWwindow* mHandle = nullptr;
};
}