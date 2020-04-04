#pragma once

class GLFWwindow;

namespace egl
{
class Window
{
public:
    Window();
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